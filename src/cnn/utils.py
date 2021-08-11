import tensorflow as tf


def weight_variable(shape, name='w'):
    initial = tf.initializers.GlorotUniform()
    return tf.Variable(initial(shape=shape), name=name)


def biases_variable(shape, name='b'):
    initial = tf.constant(0.1, shape=shape)
    return tf.Variable(initial, name=name)


def conv2d(x, W, strides=None):
    if strides is None:
        strides = [1, 1, 1, 1]
    return tf.nn.conv2d(x, W, strides=strides, padding='SAME')


def max_pool_2x2(x, k_size=None, strides=None):
    if strides is None:
        strides = [1, 2, 2, 1]
    if k_size is None:
        k_size = [1, 2, 2, 1]
    return tf.nn.max_pool(x, k_size, strides, padding='VALID')


def get_placeholder(shape):
    return tf.compat.v1.placeholder(tf.float32, shape=shape)
