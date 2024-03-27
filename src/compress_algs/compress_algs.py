import subprocess
import time
import os
import json
from pathlib import Path

def create_output_dirs():
    output_dir = Path("output/compressed_files")
    output_dir.mkdir(parents=True, exist_ok=True)
    return output_dir

def compress_measure(command_compress, input_file, output_dir, extension, needs_move=False):
    compressed_file_path = output_dir / f"{input_file.stem}.{extension}"

    print(f"command_compress: {command_compress}")
    # Measure compression
    start_time = time.time()
    subprocess.run(command_compress, shell=True)
    compression_time = time.time() - start_time

    # If the compressed file needs to be moved to the target directory
    if needs_move:
        original_compressed_path = input_file.parent / f"{input_file.stem}.txt.{extension}"
        print(f"original_compressed_path: {original_compressed_path}")
        original_compressed_path.rename(compressed_file_path)
        

    compressed_size_bits = compressed_file_path.stat().st_size * 8
    original_size_bits = Path(input_file).stat().st_size * 8
    compression_ratio = original_size_bits / compressed_size_bits

    # Remove the compressed file after measurement
    os.remove(compressed_file_path)

    return {
        "compression_ratio": compression_ratio,
        "compressed_size_bits": compressed_size_bits,
        "compression_time_seconds": compression_time,
    }

def process_file(input_file_name, output_dir):
    input_file_path = Path(f"../../example/{input_file_name}") 

    results = {}
    commands = {
        "gzip": f"gzip -c {input_file_path} > {output_dir}/{input_file_path.stem}.gz",
        "bzip2": f"bzip2 -zk {input_file_path}",
        "xz": f"xz -zk {input_file_path}",
        "lz4": f"lz4 -z {input_file_path} {output_dir}/{input_file_path.stem}.lz4",
        "zstd": f"zstd {input_file_path} -o {output_dir}/{input_file_path.stem}.zst",
    }

    needs_move = {
        "gzip": False,
        "bzip2": True,  # bzip2 needs to be moved after compression
        "xz": True,  # xz needs to be moved after compression
        "lz4": False,
        "zstd": False,
    }

    extensions = {
        "gzip": "gz",
        "bzip2": "bz2",
        "xz": "xz",
        "lz4": "lz4",
        "zstd": "zst",
    }

    for method, command in commands.items():
        results[method] = compress_measure(
            command, input_file_path, output_dir, extensions[method], needs_move=needs_move[method]
        )

    json_filename = Path("output") / f"{input_file_path.stem}_compression_metrics.json"
    with open(json_filename, "w") as f:
        json.dump(results, f, indent=4)

def main():
    output_dir = create_output_dirs()
    #input_files = ["chry.txt", "abc.txt", "test.txt", "random_file.txt"]
    input_files = ["1984_excerpt.txt"]
    
    for input_file in input_files:
        process_file(input_file, output_dir)

if __name__ == "__main__":
    main()
