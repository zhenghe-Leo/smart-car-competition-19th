# 操作指南：图片提取 & GitHub 推送

## 第一步：从 Word 文档无损提取图片

`.docx` 文件本质上是一个 ZIP 压缩包，内嵌的图片直接存放在其中，**修改后缀名解压即可实现无损提取，无需截图**。

### 方法（推荐，100% 无损）

**Windows：**
```
1. 找到「车模技术检查文档.docx」
2. 将后缀名从 .docx 改为 .zip
   → 提示"确认修改扩展名"，点击「是」
3. 双击打开该 .zip 文件
4. 进入目录：word\media\
5. 将所有图片复制出来（共 12 张）
```

**macOS / Linux：**
```bash
# 在终端中执行：
cp 车模技术检查文档.docx car_doc.zip
unzip car_doc.zip -d car_doc_extracted/
ls car_doc_extracted/word/media/
# 即可看到所有图片文件
```

### 图片命名对照表

解压后按下表重命名，然后放入仓库对应路径：

| 原始文件名 | 重命名为 | 存放路径 | 内容 |
|:---:|:---:|:---:|:---:|
| image1.jpeg | car_top.jpg | images/car_photos/ | 车模俯视图 |
| image2.jpeg | car_left.jpg | images/car_photos/ | 车模左视图 |
| image3.jpeg | car_front.jpg | images/car_photos/ | 车模前视图 |
| image4.jpeg | pcb_main_front.jpg | images/pcb_photos/ | 主板正面 |
| image5.jpeg | pcb_main_back.jpg | images/pcb_photos/ | 主板背面 |
| image6.jpeg | pcb_opamp_front.jpg | images/pcb_photos/ | 运放板正面 |
| image7.jpeg | pcb_opamp_back.jpg | images/pcb_photos/ | 运放板背面 |
| image8.jpeg | pcb_driver_front.jpg | images/pcb_photos/ | 驱动板正面 |
| image9.jpeg | pcb_driver_back.jpg | images/pcb_photos/ | 驱动板背面 |
| image10.png | schematic_main.png | images/schematics/ | 主板原理图 |
| image11.png | schematic_opamp.png | images/schematics/ | 运放板原理图 |
| image12.png | schematic_driver.png | images/schematics/ | 驱动板原理图 |

---

## 第二步：整理本地仓库文件夹

完成后，你的本地文件夹结构应该如下：

```
smart-car-competition/          ← 仓库根目录（文件夹名自定）
├── README.md
├── .gitignore
├── src/
│   ├── main.c
│   ├── Inductor.c
│   ├── ADC.c
│   ├── PID.c
│   ├── element.c
│   └── isr.c
├── project/
│   └── Seekfree_STC32F12K_Opensource_Library/   ← 解压 zip 后放这里
├── images/
│   ├── car_photos/
│   │   ├── car_top.jpg
│   │   ├── car_left.jpg
│   │   └── car_front.jpg
│   ├── pcb_photos/
│   │   ├── pcb_main_front.jpg  ... (共6张)
│   └── schematics/
│       ├── schematic_main.png  ... (共3张)
└── docs/
    └── 车模技术检查表.pdf       ← 可选：将 docx 另存为 pdf 放这里
```

---

## 第三步：推送到 GitHub

### 3.1 在 GitHub 上新建仓库

1. 登录 [github.com](https://github.com)，点击右上角 **「+」→「New repository」**
2. 填写信息：
   - Repository name：`smart-car-competition-19th`（或你喜欢的名称）
   - Description：`第十九届全国大学生智能汽车竞赛折线电磁组 | 山东赛区二等奖`
   - Visibility：`Public`（便于面试官查看）
   - **不要勾选** Initialize with README（因为我们本地已有）
3. 点击 **「Create repository」**，记下仓库地址（HTTPS 格式）

### 3.2 本地初始化并推送

```bash
# 进入你的仓库根目录
cd smart-car-competition/

# 初始化 Git 仓库
git init

# 添加所有文件
git add .

# 查看将要提交的文件（确认无误）
git status

# 提交
git commit -m "feat: 初始化项目，添加全部源码、PCB图片及说明文档"

# 关联远程仓库（替换 YOUR_USERNAME 和 REPO_NAME）
git remote add origin https://github.com/YOUR_USERNAME/REPO_NAME.git

# 推送到 main 分支
git branch -M main
git push -u origin main
```

### 3.3 验证推送结果

推送完成后，访问 `https://github.com/YOUR_USERNAME/REPO_NAME`，应能看到：
- ✅ README.md 正常渲染，徽章和图片全部显示
- ✅ 所有图片在仓库文件树中可见
- ✅ src/ 目录下的源码文件可以浏览

---

## 常见问题

**Q: 图片在 README 中不显示？**
确保图片路径大小写与实际文件名完全一致，GitHub 路径区分大小写。

**Q: Push 报错 `fatal: remote origin already exists`？**
```bash
git remote set-url origin https://github.com/YOUR_USERNAME/REPO_NAME.git
```

**Q: 文件过大推送失败？**
单文件不超过 100MB 即可正常推送。若有大文件，考虑使用 [Git LFS](https://git-lfs.github.com/)。

**Q: 想后续更新代码怎么办？**
```bash
git add .
git commit -m "fix: 调整 PID 参数"
git push
```
