import tensorflow as tf
mnist = tf.keras.datasets.mnist


def train_sample():
    (x_train, y_train),(x_test, y_test) = mnist.load_data()
    print(x_train)
    x_train, x_test = x_train / 255.0, x_test / 255.0

    print(x_train)
    print(len(x_train))
    print(len(x_train[0]))
    print(len(x_train[0][0]))
    print(len(x_train[0][0][0]))

    model = tf.keras.models.Sequential([
        tf.keras.layers.Flatten(),
        tf.keras.layers.Dense(512, activation=tf.nn.relu),
        tf.keras.layers.Dropout(0.2),
        tf.keras.layers.Dense(10, activation=tf.nn.softmax)
    ])
    model.compile(optimizer='adam',
                  loss='sparse_categorical_crossentropy',
                  metrics=['accuracy'])

    model.fit(x_train, y_train, epochs=5)
    model.evaluate(x_test, y_test)


def train(x_train, y_train):
    print(len(x_train))
    print(len(y_train))
    model = tf.keras.models.Sequential([
        tf.keras.layers.Flatten(),
        tf.keras.layers.Dense(512, activation=tf.nn.relu),
        tf.keras.layers.Dropout(0.2),
        tf.keras.layers.Dense(10, activation=tf.nn.softmax)
    ])
    model.compile(optimizer='adam',
                  loss='sparse_categorical_crossentropy',
                  metrics=['accuracy'])
    model.fit(x_train, y_train, epochs=5)


#train_sample()
