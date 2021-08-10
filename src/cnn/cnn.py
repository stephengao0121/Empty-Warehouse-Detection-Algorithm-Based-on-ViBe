import utils
import validation as vd
import tensorflow as tf
import cv2 as cv
import os
import numpy as np


def main():
    tf.compat.v1.disable_eager_execution()
    xs = utils.get_placeholder([None, 480, 520, 1])
    ys = utils.get_placeholder([None, 1])
    keep_prob = tf.compat.v1.placeholder(tf.float32)

    # Convolution #1. Kernel shape = 5*5, stride = 1.
    W_conv1 = utils.weight_variable([5, 5, 1, 32], name='w1')
    b1 = utils.biases_variable([32], name='b1')
    conv2_1 = tf.nn.relu(utils.conv2d(xs, W_conv1) + b1)
    pool1 = utils.max_pool_2x2(conv2_1)  # 238*258

    # Convolution #2. Kernel shape = 10*10, stride = 5.
    W_conv2 = utils.weight_variable([10, 10, 32, 64], name='w2')
    b2 = utils.biases_variable([64], name='b2')
    conv2_2 = tf.nn.relu(utils.conv2d(pool1, W_conv2, strides=[1, 5, 5, 1]) + b2)
    pool2 = utils.max_pool_2x2(conv2_2)  # 24*26

    W_fc1 = utils.weight_variable([24*26*64, 512], name='w3')
    b_fc1 = utils.biases_variable([512], name='b3')
    pool2_flat = tf.reshape(pool2, [-1, 24*26*64])
    fc1 = tf.nn.relu(tf.matmul(pool2_flat, W_fc1) + b_fc1)
    fc1_drop = tf.nn.dropout(fc1, keep_prob)

    W_fc2 = utils.weight_variable([512, 1], name='w4')
    b_fc2 = utils.biases_variable([1], name='b4')
    prediction = tf.nn.softmax(tf.matmul(fc1_drop, W_fc2) + b_fc2)
    correct_prediction = tf.equal(tf.argmax(ys, 1), tf.argmax(prediction, 1))

    loss = tf.losses.categorical_crossentropy(ys, prediction)
    train_step = tf.compat.v1.train.AdamOptimizer(0.00001).minimize(loss)

    sess = tf.compat.v1.Session()
    sess.run(tf.compat.v1.global_variables_initializer())

    # Training process.
    # Reading in the training set.
    dataset = []
    path = "./dataset"
    directory = os.listdir(path)
    os.chdir(path)
    for i in directory:
        os.chdir("./" + i)
        names = os.listdir("./below")
        os.chdir("./below")
        for j in names:
            image = cv.imread(j)
            image = cv.cvtColor(image[:, 120: 640], cv.COLOR_RGB2GRAY)
            if i == 'empty':
                dataset.append([np.reshape(image, [1, 480, 520, 1]) / 255, [[0]]])
            else:
                dataset.append([np.reshape(image, [1, 480, 520, 1]) / 255, [[1]]])
        os.chdir("../..")
    # Training.
    np.random.shuffle(dataset)
    for i in range(len(dataset)):
        sess.run(train_step, feed_dict={xs: dataset[i][0], ys: dataset[i][1], keep_prob: 0.5})
        print(sess.run(prediction, feed_dict={xs: dataset[i][0], keep_prob: 0.5}))
        print(sess.run(ys, feed_dict={xs: dataset[i][0], ys: dataset[i][1], keep_prob: 0.5}))
        print(sess.run(loss, feed_dict={xs: dataset[i][0], ys: dataset[i][1], keep_prob: 0.5}))
        if i % 10 == 0:
            print("{} pictures trained.".format(i))

    print("Training done.")
    exit(0)

    # Prediction.
    os.chdir("..")
    name = "配送机器人-下仓体.avi"
    video = cv.VideoCapture(name)
    frames = []
    while video.isOpened():
        ret, frame = video.read()
        frames.append(frame[:, 120: 640])
        if len(frames) == video.get(7):
            break
    print('video loaded.')
    arr = [0, 0, 0, 0]
    counter = 1
    for i in frames:
        img = cv.cvtColor(i, cv.COLOR_RGB2GRAY)
        img = np.reshape(img, [1, 480, 520, 1]) / 255
        y_pre = sess.run(prediction, feed_dict={xs: img, keep_prob: 0.5})
        vd.validate(counter, y_pre[0][0], arr)
        counter += 1
        if counter % 100 == 0:
            print("{} frames predicted.".format(counter))
    print(arr)


if __name__ == '__main__':
    main()
