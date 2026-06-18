# Removing Electrical Noise Coupling via Deep Learning Techniques

Michigan Technological University **CS/EE5841 Final Project** by **Jah Multani, Jevin Barnett, and Kyle Hildebrand**.

## Project Overview

Electrical signals measured in Electronic Control Unit (ECU) environments can contain unwanted disturbances caused by switching activity, asynchronous controllers, grounding differences, and nearby power electronics. This project evaluates whether compact deep-learning models can suppress those disturbances while remaining practical for embedded deployment.

The project compares classical filtering methods with neural-network approaches and includes:

- data loading, cleaning, and chronological preprocessing;
- sliding-window feature generation;
- residual multilayer perceptron (MLP) and 1D CNN experiments;
- supplementary LSTM, denoising-autoencoder, moving-average, and IIR experiments;
- model evaluation using MSE, RMSE, MAE, and error attenuation;
- TensorFlow Lite conversion and quantization;
- an STM32CubeIDE/X-CUBE-AI deployment project with cycle-based timing; and
- an IEEE-format report, presentation, recording, and self-declaration.

## Primary Experiment

The primary reproducible experiment is contained in `Jah_Multani_FP_NB.ipynb` and uses `Book2.xlsx`.

| Item | Value |
|---|---|
| Noisy predictor | `Sample 2` |
| Desired target | `clean2` |
| Reference column | `clean` |
| Ordered rows | 63,959 |
| Variable types | 4 continuous, 0 categorical |
| Sliding-window size | 32 samples |
| Generated windows | 63,928 |
| Learning task | Zero-reference disturbance cancellation |

The `clean` and `clean2` columns are zero throughout the main dataset. Therefore, the primary experiment demonstrates learned cancellation toward a known zero reference. It does **not** establish reconstruction of an arbitrary time-varying desired waveform.

## Repository Contents

```text
CS-EE-5841-Final-Project/
├── README.md
├── requirements.txt
├── Book2.xlsx
├── Sample_Data_1.xlsx
├── Jah_Multani_FP_NB.ipynb
├── Kyle_Hildebrand_FP_NB.ipynb
├── ENcoderDecodeCNN.ipynb
├── Untitled-1.ipynb
├── CS5841_FP.tex
├── CS5841_FP.pdf
├── Self-Declaration.tex
├── Self-Declaration.pdf
├── CS5841 FP Presentation.pdf
├── CS5841 FP Recording.mp4
└── AITest/
    ├── AITest.ioc
    ├── Core/
    ├── Drivers/
    ├── Middlewares/
    ├── X-CUBE-AI/
    ├── .ai/
    └── Debug/
```

### Important files

| Path | Description |
|---|---|
| `Jah_Multani_FP_NB.ipynb` | Main Book2 preprocessing, residual MLP, 1D CNN, evaluation, and model-export notebook |
| `Kyle_Hildebrand_FP_NB.ipynb` | Supplementary model and signal-processing experiments |
| `ENcoderDecodeCNN.ipynb` | Denoising-autoencoder experiment |
| `Book2.xlsx` | Primary 63,959-row dataset |
| `Sample_Data_1.xlsx` | Additional paired noisy/hardware-corrected dataset used in supplementary experiments |
| `CS5841_FP.tex` | IEEE conference report source |
| `CS5841_FP.pdf` | Compiled final report |
| `Self-Declaration.tex` | Individual-contribution declaration source |
| `Self-Declaration.pdf` | Compiled self-declaration |
| `AITest/` | STM32CubeIDE project containing X-CUBE-AI-generated network code and timing instrumentation |
| `requirements.txt` | Python dependencies required by the notebooks |

## Data Description

`Book2.xlsx` contains four numeric continuous variables and no categorical variables:

- `Sample 1`
- `Sample 2`
- `clean`
- `clean2`

The main predictor, `Sample 2`, has approximately the following distribution:

| Statistic | Value |
|---|---:|
| Count | 63,959 |
| Mean | 0.009434 |
| Standard deviation | 0.644899 |
| Minimum | -10.005383 |
| 25th percentile | -0.270466 |
| Median | 0.008876 |
| 75th percentile | 0.288351 |
| Maximum | 9.271493 |

Most values remain near zero, while a smaller number of large positive and negative excursions create a heavy-tailed disturbance pattern.

## Processing and Modeling Pipeline

1. Load the Excel workbook with `pandas`.
2. Validate the required column names.
3. Convert selected columns to numeric values.
4. Remove invalid or missing rows.
5. Preserve chronological order.
6. Create 32-sample sliding windows.
7. Divide the ordered windows into training, validation, and test partitions.
8. Fit input and residual-target standardization using training data only.
9. Train residual MLP and 1D CNN models.
10. Compare learned models with raw-signal and moving-average baselines.
11. Evaluate held-out predictions.
12. Export selected models to Keras and TensorFlow Lite formats.
13. Import a quantized model into X-CUBE-AI and measure embedded inference cycles.

The residual-learning formulation is:

```text
correction      = desired signal - noisy signal
predicted clean = noisy signal + predicted correction
```

For the primary dataset, the desired signal is zero, so the ideal residual correction is the negative of the noisy measurement.

## Model Architectures

### Residual MLP

```text
32 -> 64 -> 64 -> 32 -> 1
```

- ReLU hidden activations
- Linear output layer
- 8,385 trainable parameters
- Adam optimizer
- Mean-squared-error loss

### Residual 1D CNN

The CNN uses one-dimensional convolution, batch normalization, pooling, global average pooling, a dense hidden stage, and a linear residual output.

### Supplementary models

The team also evaluated:

- LSTM-based sequence modeling;
- a convolutional denoising autoencoder;
- moving-average filters; and
- an IIR low-pass filter.

## Main Held-Out Results

Results from the primary `Book2.xlsx` experiment:

| Method | RMSE | MAE | Error attenuation |
|---|---:|---:|---:|
| Raw signal | 0.95065 | 0.51153 | 0.00 dB |
| Moving average, 5 samples | 0.44343 | 0.30017 | 6.62 dB |
| Moving average, 15 samples | 0.27805 | 0.20303 | 10.68 dB |
| Residual 1D CNN | 0.15790 | 0.11395 | 15.59 dB |
| Residual MLP | 0.000464 | 0.000127 | 66.24 dB |
| Constant zero reference | 0 | 0 | N/A |

The constant-zero baseline is included because it is the exact target for the primary dataset. Consequently, the MLP result should be interpreted as learned zero-reference cancellation rather than proof of general clean-waveform reconstruction.

Standard signal-to-noise ratio is not meaningful for this experiment because the desired signal has zero power. The report therefore uses error attenuation relative to the raw signal.

## STM32 / X-CUBE-AI Deployment

The `AITest/` directory is a complete STM32CubeIDE project configured for an **STM32F303K8** Cortex-M4 target running at **64 MHz**. It contains:

- STM32CubeMX project configuration in `AITest.ioc`;
- generated HAL and startup code;
- X-CUBE-AI middleware and generated network sources;
- model-analysis metadata under `AITest/.ai/`;
- an embedded test vector;
- DWT cycle-counter timing; and
- debugger-visible inference variables.

The currently generated `network.*` source identifies the active model as `autoencoder_quant_4`, with 16 floating-point input values and 16 floating-point output values. The `.ai/` directory also contains X-CUBE-AI analysis metadata for multiple quantized autoencoder and residual-MLP variants.

`AITest/Core/Src/main.c`:

1. initializes the X-CUBE-AI network;
2. loads a stored signal window;
3. runs one forward pass;
4. measures execution cycles using the Cortex-M4 DWT counter;
5. converts the cycle count to milliseconds; and
6. exposes the result through debugger variables.

Useful STM32CubeIDE **Live Expressions**:

```text
prediction
start_cycles
stop_cycles
total_inference_cycles
execution_time_ms
```

### Build the embedded project

1. Install STM32CubeIDE with X-CUBE-AI support.
2. Open STM32CubeIDE.
3. Select **File -> Open Projects from File System**.
4. Choose the repository's `AITest/` directory.
5. Allow STM32CubeIDE to load `AITest.ioc` and the generated project files.
6. Build the `Debug` configuration.
7. Connect the configured STM32 target.
8. Start a debug session and resume execution.
9. Inspect the Live Expressions listed above.

The embedded project uses a stored test vector rather than live ADC acquisition. End-to-end acquisition and real-time signal streaming remain future work.

## Python Environment Setup

Clone the repository:

```bash
git clone https://github.com/JSMultani/CS-EE-5841-Final-Project.git
cd CS-EE-5841-Final-Project
```

Create a virtual environment:

```bash
python -m venv .venv
```

Activate it on Windows:

```bash
.venv\Scripts\activate
```

Activate it on macOS or Linux:

```bash
source .venv/bin/activate
```

Install dependencies:

```bash
python -m pip install --upgrade pip
pip install -r requirements.txt
```

The dependency file includes NumPy, pandas, Matplotlib, scikit-learn, SciPy, TensorFlow, OpenPyXL, and IPython/Jupyter support.

## Run the Primary Notebook

Start Jupyter:

```bash
jupyter notebook
```

Open:

```text
Jah_Multani_FP_NB.ipynb
```

Then:

1. Confirm that `Book2.xlsx` is accessible.
2. Update the Google Drive path or use the notebook's manual-upload fallback.
3. Restart the kernel or Colab runtime.
4. Run every cell from top to bottom.
5. Save the notebook with its generated outputs.

Expected dataset information:

```text
Columns: Sample 1, Sample 2, clean, clean2
Shape:   (63959, 4)
```

Expected sliding-window input shape:

```text
(63928, 32)
```

The notebook includes an **Open in Colab** badge for direct execution from GitHub.

## Reproducibility Configuration

| Setting | Value |
|---|---|
| NumPy seed | 42 |
| TensorFlow seed | 42 |
| Split | 70% training, 15% validation, 15% testing |
| Window size | 32 |
| Batch size | 32 |
| Optimizer | Adam |
| Initial learning rate | 0.001 |
| Loss | Mean squared error |
| Maximum epochs | 300 |
| Learning-rate patience | 10 |
| Early-stopping patience | 25 |

For independent model runs, create a fresh callback list for each `model.fit()` call so callback state is not carried between models.

## Report and Presentation

The repository contains:

- the IEEE-format report source and compiled PDF;
- the required self-declaration source and PDF;
- the final presentation PDF; and
- the recorded final presentation.

The report is formatted in the IEEE two-column conference style and documents the dataset size, variable types, predictor distribution, preprocessing, feature construction, model design, evaluation, embedded deployment, limitations, and future work.

## Known Limitations

- The main desired target is constant zero, so a constant-zero output is mathematically optimal.
- The current primary model has not demonstrated adaptation to arbitrary desired signals.
- Current measurements do not cover every ECU speed, load, temperature, and interference condition.
- The primary notebook creates windows before splitting, which causes limited overlap near partition boundaries.
- The preliminary CNN experiment reused callback objects after MLP training.
- The embedded test uses stored windows rather than a live ADC stream.
- The active generated STM32 network and the primary Book2 residual-MLP experiment are separate implemented experiments and should not be interpreted as the same trained model.

## Individual Contributions

- **Jah Multani:** Developed the primary data-processing and residual-learning notebook, implemented and evaluated the MLP and 1D CNN models, analyzed model performance, and contributed to the report and presentation.
- **Jevin Barnett:** Contributed to data collection and preparation, classical filtering methods, additional neural-network experiments, results analysis, and preparation of the report and presentation.
- **Kyle Hildebrand:** Contributed to the denoising autoencoder and IIR filter experiments, STM32 deployment and timing analysis, interpretation of embedded results, and preparation of the report and presentation.

All team members reviewed the final implementation, report, repository, and presentation.

## Authors

- Jah Multani
- Jevin Barnett
- Kyle Hildebrand

## Course

Michigan Technological University — CS/EE5841 Final Project
