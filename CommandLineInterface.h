#pragma once
#include <map>
#include <string>
#include <iostream>
#include <vector>
#include "CommonTypes.h"

#define COMMAND_ERROR "Ошибка команды: "
#define INPUT_ERROR "Ошибка ввода: "

/// ToDo||| сделать макросы обертки для инициализации команд,
/// ToDo||| написать mann прямо тут в теле cli,
/// ToDo||| написать help как функцию по доступным командам окружения,
/// ToDo||| и help как опцию для команды чтобы получить её инфу, тоже всё тут, главное не менять счётчик команд
/// ToDo||| изменить конструктор класса команды и добавить туда описание команды и описания её аргументов,
/// ToDo||| по колличеству описаний проверять массив аргументов и отменять исполнение если аргументов не то же колличество,
/// ToDo||| передавать тип структуры интерфейса программы.

class CommandLineInterface {

public:

    explicit CommandLineInterface(void* work_data, void(*startingAction)());

    enum SpecialSpaceStatements{
        final_executable_space = 0,
        primal_executable_space = -1,
        any_executable_space = -2,
        previous_executable_space = -3,
        same_executable_space = -4,
    };

    void addCommand(int new_space_ID,
                    int executable_space_ID,
                    const std::string & syntax,
                    void(*commandRealization)(void*, const std::vector<std::string>&));

    void commandPolling();

    [[maybe_unused]] static bool showNotice(std::string &_msg){
        std::string answer;
        while(true){
            std::cout << _msg << '\t' << "[Y/N]" << std::endl;
            std::cin >> answer;
            toLower(answer);
            if(answer == "y") return true;
            if(answer == "n") return false;
            std::cout << INPUT_ERROR << std::endl;
        }
    }

private:

    class Command{
        Command(int new_space_ID,
                int executable_space_ID,
                void(*commandRealization)(void*, const std::vector<std::string>&));

    private:
        int new_space_ID;
        int executable_space_ID;
        void(*callback)(void*, const std::vector<std::string>&);

        friend CommandLineInterface;
    };

    std::vector<std::string> Args;
    std::map<std::string, Command> CommandPool;
    void(*m_main_frame)();
    int m_current_space_ID;
    void* processing_struct;
};

