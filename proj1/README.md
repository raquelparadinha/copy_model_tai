# tai_grupo11

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

