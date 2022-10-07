#include "CommandLineInterface.h"

CommandLineInterface::CommandLineInterface(void *work_data, void (*startingAction)()) {
    processing_struct = work_data;
    m_current_space_ID = SpecialSpaceStatements::primal_executable_space;
    m_main_frame = startingAction;
}

void CommandLineInterface::addCommand(int new_space_ID,
                                    int executable_space_ID,
                                    const std::string & syntax,
                                    void(*commandRealization)(void*, const std::vector<std::string>&)){

    CommandPool.insert({syntax, Command(new_space_ID, executable_space_ID, commandRealization)});
}

void CommandLineInterface::commandPolling() {
    std::string console_input, current_command, arg;
    std::map<std::string, Command> LocalCommandPool;
    std::map<std::string, Command> ElderLocalCommandPool;

    m_current_space_ID = primal_executable_space;
    while (true) {
        try {
            if(m_current_space_ID == SpecialSpaceStatements::final_executable_space){
                return;
            }

            Args = std::vector<std::string>();
            LocalCommandPool.clear();
            ElderLocalCommandPool.clear();

            if(m_current_space_ID == primal_executable_space)
                m_main_frame();

            console_input = std::string();
            current_command = std::string();
            arg = std::string();

            std::getline(std::cin, console_input);
            console_input.push_back(' ');

            // input parser
            for (bool break_symbol = false; auto e: console_input) {

                if ((e == '\n' || e == ' ') && break_symbol) {
                    Args.emplace_back(arg);
                    arg = std::string();
                    continue;
                }

                if (e == ' ') { break_symbol = true; continue;}

                if (!break_symbol) current_command.push_back(e);
                else {
                    arg.push_back(e);
                }
            }

            for(auto const &e : CommandPool){
                if(e.second.executable_space_ID == m_current_space_ID
                   || e.second.executable_space_ID == SpecialSpaceStatements::any_executable_space)
                    LocalCommandPool.insert(e);
            }

            if(current_command == "help" && Args.empty()){
                for(auto const &e : LocalCommandPool){
                    std::cout << e.first << '\t';
                }
                std::cout << std::endl;
                continue;
            }

            for(auto const &e : CommandPool){
                if(e.second.new_space_ID == m_current_space_ID)
                    ElderLocalCommandPool.insert(e);
            }

            if(!LocalCommandPool.contains(current_command)){
                Args = std::vector<std::string>();
                LocalCommandPool.clear();
                throw std::invalid_argument("Нет такой команды");
            }

            auto executableStatement = LocalCommandPool.at(current_command);

            if(executableStatement.new_space_ID == SpecialSpaceStatements::previous_executable_space){
                if(ElderLocalCommandPool.empty()) m_current_space_ID = final_executable_space;
                for(auto const &e : ElderLocalCommandPool){
                    if(m_current_space_ID == primal_executable_space) {
                        m_current_space_ID = final_executable_space;
                        break;
                    }
                    if(e.second.new_space_ID == m_current_space_ID
                       && e.second.new_space_ID != e.second.executable_space_ID
                       && e.second.new_space_ID != SpecialSpaceStatements::same_executable_space) {
                        executableStatement.callback(processing_struct,Args);
                        m_current_space_ID = e.second.executable_space_ID;
                        break;
                    }
                    throw std::invalid_argument("Предыдущая команда не найдена");
                }
                continue;
            }

            executableStatement.callback(processing_struct,Args);
            if(executableStatement.new_space_ID != SpecialSpaceStatements::same_executable_space)
                m_current_space_ID = executableStatement.new_space_ID;
        }
        catch(std::exception& e){
            std::cout << COMMAND_ERROR << e.what() << std::endl;
        }
    }
}

CommandLineInterface::Command::Command(int new_space_ID, int executable_space_ID,
                                       void (*commandRealization)(void *, const std::vector<std::string> &)) {
    this->new_space_ID = new_space_ID;
    this->executable_space_ID = executable_space_ID;
    this->callback = commandRealization;
}
