#include <iostream>
#include <nats/nats.h>
#include <chrono>
#include <thread>

using namespace std;

void handler(natsConnection* conn, natsSubscription* sub, natsMsg* msg, void* closure) {
    // Получаем текущее время и округляем его до мс
    auto t_real = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();
    auto t_real_rounded = static_cast<double>(t_real) / 1000;

    // Получаем t_sign из полученного сообщения
    auto t_sign = stod(natsMsg_GetData(msg));

    // Вычисляем dt и округляем его до мс
    auto dt = t_real_rounded - t_sign;
    auto t_dt_rounded = static_cast<double>(static_cast<int>(dt * 1000)) / 1000;

    // Выводим dt в консоль
    cout << "dt = " << t_dt_rounded << endl;
}

int main() {
    // Подключаемся к серверу NATS
    natsConnection* conn = nullptr;
    natsOptions* opts = nullptr;
    natsOptions_Create(&opts);

    // Задаем соединение с сервером NATS
    natsOptions_SetURL(opts, ("nats://" + std::getenv("NATS_IP") + ":4222").c_str());

    // Устанавливаем соединение
    natsConnection_Connect(&conn, opts);

    // Создаем подписку на топик от второго приложения
    natsSubscription* sub = nullptr;
    natsConnection_Subscribe(&sub, conn, "app2_tick", handler, nullptr);

    while (true) {
        // Получаем текущее время и округляем его до мс
        auto t_real = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();
        auto t_rounded = static_cast<double>(t_real) / 1000;

        // Публикуем сообщение с округленным временем в шину данных
        natsConnection_PublishString(conn, "app1_tick", to_string(t_rounded).c_str());

        // Ставим задержку между публикациями в 10 мс
        this_thread::sleep_for(chrono::milliseconds(10));
    }

    // Освобождаем ресурсы
    natsConnection_Destroy(conn);
    natsOptions_Destroy(opts);

    return 0;
}
