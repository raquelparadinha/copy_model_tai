# Copy Model

## Team

| NMec   | Email                 | Name                                                   |
|--------|-----------------------|--------------------------------------------------------|
| 102491 | raquelparadinha@ua.pt | [Raquel Paradinha](https://github.com/raquelparadinha) |
| 103234 | paulojnpinto02@ua.pt  | [Paulo Pinto](https://github.com/Pjnp5)                |
| 103341 | miguelamatos@ua.pt    | [Miguel Matos](https://github.com/mankings)            |
| 103470 | fantao@ua.pt          | [Filipe Antão](https://github.com/fantao)              |

## Instructions

### Compilation 
From the root folder of the repo, compile the binaries:
```bash
make all
```

#### CPM
Run the CPM program:
```bash
bin/cpm -f <filename> -k <length of sequence> -a <alpha> -t <threshold> -w <fallback window size>
```

#### CPM Tuning
CPM Tuning workflow:
```bash
# (optional) tweak reference values and recompile the binaries
bin/cpm_tuning
pushd src/plotting
python3 plots.py
popd
```

#### Mutate
Run the Mutate program:
```bash
bin/mutate <filename> <mutation probability>
```

