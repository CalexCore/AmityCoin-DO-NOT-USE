import json

with open('data.json') as f:
    data = json.load(f)

f = open("data.dat", "w")
c = open("data.csv", "w")

rows=set([])
cols=set([])

for dataset in data["benchmarks"]:
    iterations = int(dataset["Iterations"])
    pagesize = int(dataset["PageSize"])
    rows.add(iterations)
    cols.add(iterations)

for row in rows:
    for col in cols:
        for dataset in data["benchmarks"]:
            iterations = int(dataset["Iterations"])
            pagesize = int(dataset["PageSize"])
            cpuTime = dataset["cpu_time"]
            if (row == iterations) and (col == pagesize):
                c.write(str(cpuTime) + ',')
                f.write(str(cpuTime) + '    ')
    f.write('\n')
    c.write('\n')
