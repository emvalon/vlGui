import numpy as np
from PIL import Image
import matplotlib.pyplot as plt
import os

# --- 读取图片并转为黑白
def load_image_to_array(filename, size=(128, 64)):
    img = Image.open(filename).convert('L')  # 灰度
    img = img.resize(size)
    img = img.point(lambda p: 255 if p > 128 else 0)  # 二值化
    array = np.array(img)
    array = np.where(array == 255, 0, 1)  # 反转: 1是黑，0是白
    return array

# --- 保存成bitmap
def save_array_to_image(array, out_filename):
    array = np.where(array == 1, 0, 255).astype(np.uint8)  # 转回白255黑0
    img = Image.fromarray(array, mode='L')
    img.save(out_filename)
    print(f"Saved to {out_filename}")

# --- 交互式像素编辑器
def pixel_editor(array, filename):
    fig, ax = plt.subplots()
    img = ax.imshow(array, cmap='gray', interpolation='nearest')
    plt.title('Click to edit pixels, Close window to save')

    # 点击事件处理
    def onclick(event):
        if event.xdata is not None and event.ydata is not None:
            x = int(round(event.xdata))
            y = int(round(event.ydata))
            if 0 <= x < array.shape[1] and 0 <= y < array.shape[0]:
                array[y, x] = 1 - array[y, x]  # 取反
                img.set_data(array)
                fig.canvas.draw()

    cid = fig.canvas.mpl_connect('button_press_event', onclick)
    plt.show()

    # 保存文件
    save_array_to_image(array, filename)

# --- 主流程
def main(input_file):
    # 去掉路径，只用文件名
    filename_only = os.path.basename(input_file)
    name, _ = os.path.splitext(filename_only)

    output_dir = "bitmap"
    if not os.path.exists(output_dir):
        os.makedirs(output_dir)

    output_file = os.path.join(output_dir, f"{name}.bmp")

    arr = load_image_to_array(input_file, size=(128, 64))  # 可以改尺寸
    pixel_editor(arr, output_file)

if __name__ == "__main__":
    input_path = "Shirop5.png"  # <<< 替换成你的图片路径
    main(input_path)
