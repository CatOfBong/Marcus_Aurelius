
import os
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from scipy.interpolate import griddata

# Настройка путей
BASE_DIR = os.path.dirname(os.path.abspath(__file__))  # Директория скрипта
DATA_PATH = os.path.join(BASE_DIR, '126.csv')     # Путь к данным
OUTPUT_DIR = os.path.join(BASE_DIR, 'output_126')         # Папка для графиков

# Создаем папку для результатов, если её нет
os.makedirs(OUTPUT_DIR, exist_ok=True)

def load_data(file_path):
    """Загрузка и проверка данных"""
    if not os.path.exists(file_path):
        raise FileNotFoundError(f"Файл данных не найден: {file_path}")
    
    df = pd.read_csv(file_path)
    
    # Проверяем необходимые колонки
    required_columns = ['m', 'I', 'V_ub']
    if not all(col in df.columns for col in required_columns):
        missing = set(required_columns) - set(df.columns)
        raise ValueError(f"Отсутствуют колонки: {missing}")
        
    return df

def plot_velocity_profile(data, m_value, output_dir):
    """Создание и сохранение графика распределения скорости по расстоянию"""
    plt.figure(figsize=(10, 6))
    
    # Уникальные значения расстояния
    distances = np.sort(data['I'].unique())
    velocities = []
    
    # Для каждого расстояния берем среднюю скорость
    for dist in distances:
        vel = data[data['I'] == dist]['V_ub'].mean()
        velocities.append(vel)
    
    # Построение графика
    plt.plot(distances, velocities, 'r-', linewidth=2)
    
    # Настройка оформления
    plt.title(f"Распределение скорости осколков (масса: {m_value} г)")
    plt.xlabel("Расстояние от центра (м)")
    plt.ylabel("Скорость (м/с)")
    plt.grid(True)
    
    # Автоматическое масштабирование осей
    plt.xlim(left=0)
    plt.ylim(bottom=0)
    
    # Сохранение
    filename = os.path.join(output_dir, f'mass_{m_value:03d}.png')
    plt.savefig(filename, dpi=150, bbox_inches='tight')
    plt.close()

def main():
    # Загрузка данных
    df = load_data(DATA_PATH)
    
    # Обработка для каждой массы
    for m in df['m'].unique():
        m_data = df[df['m'] == m]
        plot_velocity_profile(m_data, m, OUTPUT_DIR)
        
    print(f"Графики сохранены в: {OUTPUT_DIR}")

if __name__ == "__main__":
    main()
