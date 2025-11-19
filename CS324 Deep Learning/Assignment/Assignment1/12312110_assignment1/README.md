# How to run Assignment 1

<br>

Requirements:
```
numpy
scikit-learn
matplotlib
```

## Part 1

Run the training script `train.py` with

```bash
python train.py
```

## Part 2

Run the training script `train_mlp_numpy.py` with

```bash
python train_mlp_numpy.py --dnn_hidden_units 20 --learning_rate 0.01 --max_steps 1500 --eval_freq 10
```

For visualization, please run the jupyter notebook `mlp.ipynb`.

## Part 3

Run the training script `train_mlp_numpy.py` with

```bash
python train_mlp_numpy.py --dnn_hidden_units 20 --learning_rate 0.01 --max_steps 1500 --eval_freq 10 --sgd True --batch_size 1
```

and you can change the value of `sgd` and `batch_size` for Task 2.

For visualization, please run the jupyter notebook `train_mlp.ipynb`.

