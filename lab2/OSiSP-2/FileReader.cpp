#include "FileReader.h"

inline void split(const string& str, vector<string>& vector)
{
	istringstream iss(str);
	copy(istream_iterator<string>(iss), istream_iterator<string>(), back_inserter(vector));
}

FileReader::FileReader() {

}
Strings FileReader::GetContentFromFile(string path)
{
	Strings content;
	ifstream file(path);
	try {
		if (file.is_open())
		{
			string line;
			vector<string> vector;
			while (getline(file, line))
			{
				if (line != "") {
					vector.push_back(line);
				}
				else {
					content.push_back(vector);
					vector.clear();
				}
			}
			content.push_back(vector);
			vector.clear();
			file.close();
		}
		else {
			throw new exception("Файл не найден или занят другим процессом");
		}																  	
	}
	catch (...) {
	}
	return content;
}