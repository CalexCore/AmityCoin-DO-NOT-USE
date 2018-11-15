#include <random>

#include <gtest/gtest.h>

#include <config/CryptoNoteConfig.h>
#include <crypto/hash.h>
#include <CryptoNote.h>
#include <CryptoNoteCore/CachedBlock.h>
#include <CryptoNoteCore/Currency.h>
#include <CryptoNoteCore/CryptoNoteTools.h>
#include <CryptoNoteCore/DataBaseConfig.h>
#include <CryptoNoteCore/DatabaseBlockchainCache.h>
#include <CryptoNoteCore/DatabaseBlockchainCacheFactory.h>
#include <CryptoNoteCore/RocksDBWrapper.h>
#include <Logging/LoggerManager.h>
#include <Logging/ConsoleLogger.h>

class HashConsistency : public ::testing::Test {
public:
    Logging::LoggerManager LogManager;
    Logging::ConsoleLogger Log{Logging::Level::WARNING};
    CryptoNote::RocksDBWrapper DB{LogManager};
    std::unique_ptr<CryptoNote::DatabaseBlockchainCacheFactory> CacheFactory;
    std::unique_ptr<CryptoNote::IBlockchainCache> Cache;

    void SetUp() override {
        using namespace CryptoNote;

        DataBaseConfig cfg;
        cfg.setDataDir("./");
        DB.init(cfg);
        CacheFactory = std::make_unique<DatabaseBlockchainCacheFactory>(DB, Log);

        CurrencyBuilder currencyBuilder{LogManager};
        currencyBuilder.isBlockexplorer(false);
        currencyBuilder.testnet(false);
        const Currency currency = currencyBuilder.currency();
        Cache = CacheFactory->createRootBlockchainCache(currency);

        ASSERT_GT(Cache->getBlockCount(), 0u);
    }

    void TearDown() override {
        Cache.reset();
        DB.shutdown();
    }

    void TestHashes(const std::vector<Crypto::Hash>& blockHashes) {
        using namespace CryptoNote;

        for(const auto& hash : blockHashes) {
            auto blockIndex = Cache->getBlockIndex(hash);
            const auto binaryBlock = Cache->getBlockByIndex(blockIndex).block;
            const BlockTemplate blockTemplate{fromBinaryArray<BlockTemplate>(binaryBlock)};
            CachedBlock block{blockTemplate};
            Crypto::Hash blockLongHash{};
            if (blockTemplate.majorVersion == BLOCK_MAJOR_VERSION_1) {
                const auto& rawHashingBlock = block.getBlockHashingBinaryArray();
                cn_slow_hash_v0(rawHashingBlock.data(), rawHashingBlock.size(), blockLongHash);
            } else if ((blockTemplate.majorVersion == BLOCK_MAJOR_VERSION_2) || (blockTemplate.majorVersion == BLOCK_MAJOR_VERSION_3)) {
                const auto& rawHashingBlock = block.getParentBlockHashingBinaryArray(true);
                cn_slow_hash_v0(rawHashingBlock.data(), rawHashingBlock.size(), blockLongHash);
            } else if (blockTemplate.majorVersion == BLOCK_MAJOR_VERSION_4) {
                const auto& rawHashingBlock = block.getParentBlockHashingBinaryArray(true);
                cn_lite_slow_hash_v1(rawHashingBlock.data(), rawHashingBlock.size(), blockLongHash);
            } else if (blockTemplate.majorVersion >= BLOCK_MAJOR_VERSION_5) {
                const auto& rawHashingBlock = block.getParentBlockHashingBinaryArray(true);
                cn_soft_shell_slow_hash_v1(rawHashingBlock.data(), rawHashingBlock.size(), blockLongHash, blockIndex);
            } else {
                throw std::runtime_error("Unknown block major version.");
            }
            EXPECT_EQ(block.getBlockLongHash(), blockLongHash);
        }
    }
};

TEST_F(HashConsistency, First200Blocks)
{
    const auto& blockHashes = Cache->getBlockHashes(0, 200);
    TestHashes(blockHashes);
}

TEST_F(HashConsistency, Random1kBlocks)
{
    const std::size_t NumBlocks = 1000;
    std::default_random_engine rndEngine{};
    auto rndGen = std::make_unique<std::mt19937>();
    auto blockCount = Cache->getBlockCount();
    std::uniform_int_distribution<uint32_t> dist{1, blockCount - 1};
    std::vector<Crypto::Hash> blockHashes;
    blockHashes.reserve(1000);
    for(std::size_t i = 0; i < NumBlocks; ++i) {
        Cache->getBlockHash(dist(*rndGen));
    }
    TestHashes(blockHashes);
}
