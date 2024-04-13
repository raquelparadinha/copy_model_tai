import json
import matplotlib.pyplot as plt


def load_data(file_path):
    with open(file_path, 'r') as file:
        return json.load(file)

def plot_data(parameter_name, data):
    x_values = [d[parameter_name] for d in data]
    metrics = {
        'totalNumberOfBits': 'bits',
        'executionTime': 'seconds',
        'averageBitsPerSymbol': 'bits/symbol',
        'compressionRatio': 'ratio'
    }

    for metric in metrics.keys():
        y_values = [d[metric] for d in data]
        plt.figure() 
        plt.plot(x_values, y_values, label=metric)
    
        plt.xlabel(parameter_name)
        plt.ylabel(metrics[metric])
        plt.title(f'Impact of {parameter_name} on {metric}')
        plt.legend()
        
        output_folder = '../../output/plots/'
        plt.savefig(output_folder + f'{parameter_name}_{metric}.png')
        plt.close() 

if __name__ == '__main__':
    data_files = {
        'alpha': '../../output/results/results_a.json',
        'k': '../../output/results/results_k.json',
        'threshold': '../../output/results/results_t.json',
        'fallbackWindowSize': '../../output/results/results_ws.json'
    }

    for parameter, file_path in data_files.items():
        data = load_data(file_path)
        plot_data(parameter, data)
