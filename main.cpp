#include <iostream>
#include <chrono>
#include "TaskQueue.h"
#include <crow.h>

std::string readFile(const std::string& filePath) {
    std::ifstream file(filePath);
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

TaskQueue taskQueue(4); // Create a task queue with 4 worker threads

void exampleTask(int id) {
    std::cout << "Task " << id << " is starting." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(2)); // Simulate heavy task
    std::cout << "Task " << id << " is completed." << std::endl;
}

int main() {
    crow::SimpleApp app;

    CROW_ROUTE(app, "/")([]{
        std::string content = readFile("templates/index.html");
        return crow::response(content);
    });


    CROW_ROUTE(app, "/tasks").methods("POST"_method)([](const crow::request &req) {
        // Get taskId from request body
        int taskId = std::rand(); // Generate a random task ID for demonstration
        taskQueue.enqueue([taskId] { exampleTask(taskId); });
        return crow::response{201, "Task added to queue."};
    });

   

    app.port(18080).multithreaded().run();

    return 0;
}
