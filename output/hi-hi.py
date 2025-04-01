
import os
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from scipy.interpolate import griddata


BASE_DIR = os.path.dirname(os.path.abspath(__file__))  
DATA_PATH = os.path.join(BASE_DIR, '126.csv')     
OUTPUT_DIR = os.path.join(BASE_DIR, 'output_126') 

os.makedirs(OUTPUT_DIR, exist_ok=True)

def load_data(file_path):
    """Загрузка и проверка данных"""
    if not os.path.exists(file_path):
        raise FileNotFoundError(f"Файл данных не найден: {file_path}")
    
    df = pd.read_csv(file_path)
    
    required_columns = ['m', 'I', 'V_ub']
    if not all(col in df.columns for col in required_columns):
        missing = set(required_columns) - set(df.columns)
        raise ValueError(f"Отсутствуют колонки: {missing}")
        
    return df

def plot_velocity_profile(data, m_value, output_dir):
    plt.figure(figsize=(10, 6))
    
    distances = np.sort(data['I'].unique())
    velocities = []
    
    for dist in distances:
        vel = data[data['I'] == dist]['V_ub'].mean()
        velocities.append(vel)
    
    plt.plot(distances, velocities, 'r-', linewidth=2)
    
    plt.title(f" ПЭ массой {m_value} (г)")
    plt.xlabel("Расстояние от центра (м)")
    plt.ylabel("Скорость (м/с)")
    plt.grid(True)
    
    plt.xlim(left=0)
    plt.ylim(bottom=0)
    
    filename = os.path.join(output_dir, f'ПЭ_{m_value:03d}_г.png')
    plt.savefig(filename, dpi=150, bbox_inches='tight')
    plt.close()

def main():
    df = load_data(DATA_PATH)
    
    for m in df['m'].unique():
        m_data = df[df['m'] == m]
        plot_velocity_profile(m_data, m, OUTPUT_DIR)
        
    print(f"Графики сохранены в: {OUTPUT_DIR}")

if __name__ == "__main__":
    main()
