/*
 * @project Soundwave's task manager(todo)
 * @file main.cpp
 * @author SoundWave
 * @date January 23, 2020
 * @brief Main file of project with main loop
 * */

#include <iostream>
#include <string> 
#include <vector>
#include <utility>
#include <map>
#include <filesystem>
#include "../include/swtdfile.hpp"
#include "../include/print.hpp"


//global data:
std::vector<std::string> _data;
unsigned int _data_len{0};
std::filesystem::path global_file_path;
//!global data

template<typename text_type>
void init_file(text_type &path){
	global_file_path = path;
}



std::vector<std::string>  swtd_commands = {"help","add","rm","exit"};

typedef std::vector<std::string> arg_type;

int swtd_help(arg_type&);
int swtd_add(arg_type&);
int swtd_rm(arg_type&);
int swtd_exit(arg_type&);


int (*swtd_func[]) (arg_type&) = {
	&swtd_help,
	&swtd_add,
	&swtd_rm,
	&swtd_exit
};

int swtd_help(arg_type & args){
	//print help
	return 1;
}

int swtd_add(arg_type&args){
	//auto _token = args.at(1);
	std::string _token{""};
	for(std::size_t i =1;i<args.size();++i){
		_token+=args.at(i);
		_token+=' ';
	}
	addtofile(global_file_path, _token);
	return 1;
}
int swtd_rm(arg_type&args){
	uint index = std::stoi(args.at(1));
	rmfromfile(global_file_path, index);
	return 1;
}
int swtd_exit(arg_type&args){
	return 0;
}


void show(){
	std::string buffer{""};
	auto [t_, t__] = read_file(global_file_path);
	_data = std::move(t_);
	_data_len = std::move(t__);
	for(std::size_t i=0;i< _data_len;++i){
		buffer+=std::to_string((i+1));
		buffer+=") ";
		buffer+=_data.at(i);
		buffer+='\n';
	}
	cprint((char*)buffer.c_str());
}


arg_type split_line(std::string&line){
	std::stringstream ss{line};
	std::string t_;
	arg_type t__;
	while(ss >> t_){
		t__.push_back(t_);
	}
	return t__;
}

int execute(arg_type&args){
	if(args.size() == 0)
		return 1;
	for(std::size_t i= 0;i<swtd_commands.size(); ++i){
		if(args.at(0) == swtd_commands.at(i)){
			return (*swtd_func[i])(args);
		}
	}
	return 1;
}

void task_loop(){
	int status;
	do{
		show();
		std::cout << ">";
		std::string line;
		std::getline(std::cin, line);
		std::vector<std::string> argline = split_line(line);
		status = execute(argline);
	}while(status);
}


int main(){
	init_file("/root/.local/share/swtd/file.txt");
	task_loop();
}