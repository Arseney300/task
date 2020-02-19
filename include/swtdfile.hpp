/* 
 * @project Soundwave's task manager(todo)
 * @file swtdfile.hpp
 * @author Soundwave
 * @date January 23, 2020
 * @brief file description
 * */

#include <string.h>
#include <fstream>
#include <vector>
#include <utility>
#include <filesystem>

struct swtd_file{
private:
	typedef unsigned int len_type;
public:
	std::vector<std::string> tokens;
	len_type size;

	swtd_file():
		size{0}
	{}
};
template<typename path_type>
swtd_file read_file(path_type& path){
	try{
		std::ifstream file;
		file.open(path.c_str());
		if(!file.is_open()){
			fprintf(stderr, "swtd: file open error");
			swtd_file empty;
			return empty;
		}
		std::vector<std::string> tokens_array;
		//in file only one line
		std::string buffer;
		std::getline(file,buffer);
		file.close();
		for(std::size_t i = 0;i< buffer.size(); ++i){
			if(buffer.at(i) == '{'){
				//start of token
				std::string token_;
				i++;
				while(buffer.at(i)!='}'){
					if(i>=buffer.size()){
						fprintf(stderr, "swtd: file reading error");
					}
					token_+=buffer.at(i);
					i++;
				}
				tokens_array.push_back(token_);
			}
		}
		swtd_file ret_file;
		ret_file.tokens = std::move(tokens_array);
		ret_file.size = ret_file.tokens.size();
		return ret_file;
	}
	catch(std::exception &e){
		std::cerr << e.what() << std::endl;
	}

}


template <typename path_type>
void updatefile(path_type& path,  swtd_file update_data){
	try{
		std::ofstream file;
		file.open(path.c_str());
		if(!file.is_open()){
			std::cerr << "swtd: file open error";
			return;
		}
		for(auto &i: update_data.tokens){
			file << "{" << i << "}";
		}
		file.close();
	}
	catch(std::exception &e){
		std::cerr << e.what();
	}
}

template <typename path_type, typename text_type>
void addtofile(path_type& path, text_type& token){
	try{
		std::ofstream file;
		file.open(path.c_str(), std::ofstream::app);
		if(!file.is_open()){
			std::cerr << "swtd: file open error";
			return;
		}
		file << "{" << token << "}";
		file.close();
	}
	catch(std::exception&e){
		std::cerr << e.what() << std::endl;	
	}
}

template <typename path_type, typename index_type>
void rmfromfile(path_type&path, const index_type&index){
	try{
		auto file_copy_buffer = read_file(path);
		if(index > file_copy_buffer.size){
			std::cerr << "swtd: can't delete this task";
			return;
		}
		file_copy_buffer.tokens.erase(file_copy_buffer.tokens.begin() + index -1);
		updatefile(path,file_copy_buffer);
	}
	catch(std::exception e){
		std::cerr << e.what() << std::endl;
	}
}