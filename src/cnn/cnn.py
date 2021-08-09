import utils
import tensorflow as tf
import cv2 as cv
import os
import numpy as np


def main():
    tf.compat.v1.disable_eager_execution()
    xs = utils.get_placeholder([None, 480, 520, 3])
    ys = utils.get_placeholder([None, 1])
    keep_prob = tf.compat.v1.placeholder(tf.float32)

    # Convolution #1. Kernel shape = 5*5, stride = 1.
    W_conv1 = utils.weight_variable([5, 5, 3, 32], name='w1')
    b1 = utils.biases_variable([32])
    conv2_1 = tf.nn.relu(utils.conv2d(xs, W_conv1) + b1)
    pool1 = utils.max_pool_2x2(conv2_1)  # 238*258

    # Convolution #2. Kernel shape = 10*10, stride = 5.
    W_conv2 = utils.weight_variable([10, 10, 32, 64], name='w2')
    b2 = utils.biases_variable([64])
    conv2_2 = tf.nn.relu(utils.conv2d(pool1, W_conv2, strides=[1, 5, 5, 1]) + b2)
    pool2 = utils.max_pool_2x2(conv2_2)  # 24*26

    W_fc1 = utils.weight_variable([24*26*64, 512])
    b_fc1 = utils.biases_variable([512])
    pool2_flat = tf.reshape(pool2, [-1, 24*26*64])
    fc1 = tf.nn.relu(tf.matmul(pool2_flat, W_fc1) + b_fc1)
    fc1_drop = tf.nn.dropout(fc1, keep_prob)

    W_fc2 = utils.weight_variable([512, 1])
    b_fc2 = utils.biases_variable([1])
    prediction = tf.nn.softmax(tf.matmul(fc1_drop, W_fc2) + b_fc2)

    loss = tf.compat.v1.reduce_mean(-tf.compat.v1.reduce_sum(ys*tf.math.log(prediction), reduction_indices=[1]))
    train_step = tf.compat.v1.train.AdamOptimizer(1e-4).minimize(loss)

    init = tf.compat.v1.initialize_all_variables()
    sess = tf.compat.v1.Session()
    sess.run(init)

    # Training process.
    # Reading in the training set.
    dataset = []
    y = []
    path = "./dataset"
    directory = os.listdir(path)
    os.chdir(path)
    for i in directory:
        os.chdir("./" + i)
        names = os.listdir("./below")
        os.chdir("./below")
        for j in names:
            image = cv.imread(j)
            # image = cv.cvtColor(image[:, 120: 640], cv.COLOR_RGB2GRAY)
            dataset.append(np.reshape(image[:, 120: 640], [1, 480, 520, 3]))
            if i == 'empty':
                y.append([[0]])
            else:
                y.append([[1]])
        os.chdir("../..")
    # Training.
    for i in range(len(dataset)):
        sess.run(train_step, feed_dict={xs: dataset[i], ys: y[i], keep_prob: 0.5})
        if i % 10 == 0:
            print("{} pictures trained.".format(i))

    print("Training done.")

    # Prediction.
    

if __name__ == '__main__':
    main()
