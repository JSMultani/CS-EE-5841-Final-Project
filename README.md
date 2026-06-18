# Removing Electrical Noise Coupling via Deep Learning Techniques

Michigan Technological University CS5841 final project by Jah Multani, Jevin Barnett, and Kyle Hildebrand.

## Overview

This project studies whether compact neural networks can suppress electrical disturbances in Electronic Control Unit (ECU) measurements and run efficiently on an STM32-class microcontroller. Classical filtering methods are compared with residual deep-learning models.

The primary reproducible experiment is in `Jah_Multani_FP_NB.ipynb` and uses `Book2.xlsx`.

- Noisy input: `Sample 2`
- Desired target: `clean2`
- Ordered rows: 63,959
- Sliding-window size: 32 samples
- Task: zero-reference disturbance cancellation

Because `clean2` is zero throughout the main dataset, this experiment demonstrates learned cancellation toward a known zero reference. It does not prove reconstruction of an arbitrary time-varying waveform.

## Repository Files

| File | Purpose |
|---|---|
| `Jah_Multani_FP_NB.ipynb` | Main preprocessing, MLP, CNN, evaluation, and export notebook |
| `Kyle_Hildebrand_FP_NB.ipynb` | Supplementary filtering and model experiments |
| `Book2.xlsx` | Main dataset |
| `Sample_Data_1.xlsx` | Additional team dataset |
| `requirements.txt` | Python dependencies |
| `CS5841_FP_Update.tex` | IEEE report source |
| `CS5841_FP.pdf` | Report PDF |
| `CS5841 FP Presentation.pdf` | Final presentation |
| `CS5841 FP Recording.mp4` | Recorded presentation |

## Main Pipeline

1. Load the Excel data with pandas.
2. Validate and clean the required columns.
3. Preserve chronological order.
4. Create 32-sample sliding windows.
5. Fit input and target scalers using training data only.
6. Train residual MLP and 1D CNN models.
7. Compare against raw-signal and moving-average baselines.
8. Export the selected model to TensorFlow Lite.
9. Run stored test windows on STM32 and measure forward-pass time.

The residual target is:

```text
correction = desired signal - noisy signal
predicted clean = noisy signal + predicted correction
```

## Models

### Residual MLP

```text
32 -> 64 -> 64 -> 32 -> 1
```

- ReLU hidden activations
- Linear output
- 8,385 trainable parameters

### Residual 1D CNN

The CNN uses convolution, batch normalization, pooling, global average pooling, and a dense output stage. Supplementary experiments also include IIR filtering, LSTM models, and a denoising autoencoder.

## Main Results

Held-out results from `Book2.xlsx`:

| Method | RMSE | MAE | Error attenuation |
|---|---:|---:|---:|
| Raw signal | 0.95065 | 0.51153 | 0.00 dB |
| Moving average, 5 samples | 0.44343 | 0.30017 | 6.62 dB |
| Moving average, 15 samples | 0.27805 | 0.20303 | 10.68 dB |
| Residual 1D CNN | 0.15790 | 0.11395 | 15.59 dB |
| Residual MLP | 0.000464 | 0.000127 | 66.24 dB |
| Constant zero reference | 0 | 0 | N/A |

The zero-reference baseline is included because it is the exact target for this dataset. The MLP result should therefore be interpreted as learned zero-reference cancellation rather than general clean-signal reconstruction.

## STM32 Deployment

Reported MLP deployment characteristics:

- 8,385 parameters
- About 33.5 KB of float32 parameter storage
- 32-sample runtime input, equal to 128 bytes
- Approximately 35.8 KB float TensorFlow Lite model
- Approximately 13.2 KB optimized TensorFlow Lite model
- Approximately 1.7 ms quantized forward pass on STM32

The current embedded test uses stored signal windows. Live ADC acquisition is future work.

## Setup

Clone the repository:

```bash
git clone https://github.com/JSMultani/CS-EE-5841-Final-Project.git
cd CS-EE-5841-Final-Project
```

Create a virtual environment and install dependencies:

```bash
python -m venv .venv
```

Windows:

```bash
.venv\Scripts\activate
```

macOS or Linux:

```bash
source .venv/bin/activate
```

Install packages:

```bash
python -m pip install --upgrade pip
pip install -r requirements.txt
```

Start Jupyter:

```bash
jupyter notebook
```

Open `Jah_Multani_FP_NB.ipynb`, confirm that `Book2.xlsx` is available, restart the kernel, and run all cells from top to bottom.

Expected source columns:

```text
Sample 1, Sample 2, clean, clean2
```

Expected main input shape with a 32-sample window:

```text
(63928, 32)
```

## Reproducibility Settings

- NumPy seed: 42
- TensorFlow seed: 42
- Split: 70% training, 15% validation, 15% testing
- Window size: 32
- Batch size: 32
- Optimizer: Adam
- Initial learning rate: 0.001
- Loss: mean squared error
- Maximum epochs: 300
- Learning-rate patience: 10
- Early-stopping patience: 25

Create a fresh callback list for each model training run so that callback state is not reused.

## Evaluation Metrics

- Mean squared error
- Root mean squared error
- Mean absolute error
- Error attenuation relative to the raw signal
- Parameter count
- Model file size
- Inference time

Standard SNR is not meaningful for the main dataset because the target signal has zero power. Error attenuation is used instead.

## Limitations

- The main target is constant zero, so a constant-zero output is mathematically optimal.
- The model has not demonstrated adaptation to arbitrary desired signals.
- Current data does not cover every ECU speed, load, temperature, or interference condition.
- STM32 validation uses stored windows rather than live acquisition.
- The original notebook creates windows before splitting, which introduces limited overlap near split boundaries.
- The preliminary CNN run reused callback objects after MLP training; fresh callbacks should be used in future runs.

## Authors

- Jah Multani
- Jevin Barnett
- Kyle Hildebrand

## Course

Michigan Technological University, CS5841 Final Project.