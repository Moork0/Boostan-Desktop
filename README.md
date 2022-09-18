# Boostan-Desktop

There is a university affairs automation software in Iran called Golestan, which is highly legacy, outdated, and unusable. Until a few years ago, they couldn't even support browsers other than Internet Explorer.
To solve this problem, I developed free software called Boostan (Boostan and Golestan have the same meaning, "garden") that is more beautiful, easier to use, and less likely to cause headaches.

Boostan is actually a desktop UI for Golestan. It "simulates" weird browser requests to the Golestan server, processes its weird response, and renders them in a reasonable way for the user(I use the word "simulate" because we are actually doing this. There is no API or something like that. I somehow reverse-engineered the requests and responses to understand how to send/receive proper data).

Aside from Golestan's basic functionalities, Boostan provides much more features.

Boostan is written in C++, Qt, and QML.

Demo:

https://seedpuller.github.io/Boostan-Desktop/assets/img/out1.mp4

# Acknowledgement

- [Vida Azadi](mailto:azadivida@gmail.com): UI design
- [Amir TBO](https://t.me/TheBurningOne): Assisting with the design of UI for the login page and Boostan icon.
