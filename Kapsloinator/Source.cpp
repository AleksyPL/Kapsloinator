#include "opencv2\opencv.hpp"
#include <boost/filesystem.hpp>
#include <iostream>
#include <algorithm>
#include <cmath>
#include <vector>
#include <chrono>
#include <ctime>
#include <fstream>

using namespace cv;
using namespace boost::filesystem;
using std::endl;
using std::cout;
using std::cin;
using std::string;
using std::vector;
using std::fstream;
using std::ios;

string extractLastFolderName(string path)
{
	string stringToReturn = "";
	int fileNameBegin=0;
	for (int i = path.size()-1; i > 0; i--)
	{
		if (path[i] == '\\')
		{
			fileNameBegin = i;
			break;
		}
	}
	for (int i = fileNameBegin+1; i < path.size(); i++)
	{
		stringToReturn += path[i];
	}
	return stringToReturn;
}
int returnAmountOfFilesInDirectory(string pathString)
{
	boost::filesystem::path the_path(pathString);
	int cnt = std::count_if(directory_iterator(the_path), directory_iterator(), static_cast<bool(*)(const boost::filesystem::path&)>(is_regular_file));
	return cnt;
}
void returnPathsToFoldersInsideTheFolder(string sourcePath, vector<string>&insidePaths)
{
	boost::filesystem::path the_path(sourcePath);
	for (auto& p : boost::filesystem::directory_iterator(the_path))
	{
		insidePaths.push_back(p.path().string());
	}
}
string getTime()
{
	time_t rawtime;
	time(&rawtime);
	char str[26];
	ctime_s(str, sizeof str, &rawtime);
	return {str[11],str[12],str[13],str[14],str[15],str[16],str[17],str[18]};
}
string getDate()
{
	time_t rawtime;
	time(&rawtime);
	char str[26];
	ctime_s(str, sizeof str, &rawtime);
	if (str[4] == 'J' && str[5] == 'a')
		return { str[8], str[9],'.','0','1','.', str[20],str[21],str[22],str[23] };
	else if (str[4] == 'F' && str[5] == 'e')
		return { str[8], str[9],'.','0','2','.', str[20],str[21],str[22],str[23] };
	else if (str[4] == 'M' && str[5] == 'a' && str[6] == 'r')
		return { str[8], str[9],'.','0','3','.', str[20],str[21],str[22],str[23] };
	else if (str[4] == 'A' && str[5] == 'p')
		return { str[8], str[9],'.','0','4','.', str[20],str[21],str[22],str[23] };
	else if (str[4] == 'M' && str[5] == 'a' && str[6] == 'y')
		return { str[8], str[9],'.','0','5','.', str[20],str[21],str[22],str[23] };
	else if (str[4] == 'J' && str[5] == 'u' && str[6] == 'n')
		return { str[8], str[9],'.','0','6','.', str[20],str[21],str[22],str[23] };
	else if (str[4] == 'J' && str[5] == 'u' && str[6] == 'l')
		return { str[8], str[9],'.','0','7','.', str[20],str[21],str[22],str[23] };
	else if (str[4] == 'A' && str[5] == 'u')
		return { str[8], str[9],'.','0','8','.', str[20],str[21],str[22],str[23] };
	else if (str[4] == 'S' && str[5] == 'e')
		return { str[8], str[9],'.','0','9','.', str[20],str[21],str[22],str[23] };
	else if (str[4] == 'O' && str[5] == 'c')
		return { str[8], str[9],'.','1','0','.', str[20],str[21],str[22],str[23] };
	else if (str[4] == 'N' && str[5] == 'o')
		return { str[8], str[9],'.','1','1','.', str[20],str[21],str[22],str[23] };
	else if (str[4] == 'D' && str[5] == 'e')
		return { str[8], str[9],'.','1','2','.', str[20],str[21],str[22],str[23] };

}
void generateTextFile(string finalPath, string fileName, vector<string>& vectorFile)
{
	std::fstream file;
	file.open(finalPath + "\\" + fileName + ".txt", ios::out);
	for (int i = 0; i < vectorFile.size(); i++)
	{
		file << vectorFile[i] << endl;
	}
	file.close();
}
float processFilesFromDirectory(string path, int filesAmount, string finalPath, vector<string>&errorLog, vector<string>&raport)
{
	int rowsInPhoto = 0;
	int colsInPhoto = 0;
	if (filesAmount % 5 != 0)
	{
		rowsInPhoto = filesAmount / 5 + 1;
	}
	else
	{
		rowsInPhoto = filesAmount / 5;
	}
	if (filesAmount < 5)
	{
		colsInPhoto = filesAmount;
	}
	else
	{
		colsInPhoto = 5;
	}
	Mat errorImg = imread("./Error/Error.jpg", IMREAD_UNCHANGED);
	cv::Mat finalImg(rowsInPhoto * 1000, colsInPhoto * 1000, errorImg.type(), Scalar(0, 0, 0));
	cout << extractLastFolderName(path) << ": Work in progress..." << endl;
	std::chrono::duration<float> timeDuration;
	try
	{
		auto start = std::chrono::high_resolution_clock::now();
		//files
		for (int i = 0; i < filesAmount; i++)
		{
			Mat inProgressImg = imread(path + "\\" + std::to_string(i) + ".jpg", IMREAD_UNCHANGED);
			if (inProgressImg.cols != 1000 || inProgressImg.rows != 1000)
			{
				string message = getDate() + " " + getTime() + " Size or name of the picture nr. " + std::to_string(i) + " in " + extractLastFolderName(path) + " folder is incorrect. Skipping picture.";
				errorLog.push_back(message);
				cout << message << endl;
				inProgressImg = errorImg;
			}
			//rows
			for (int j = 0; j < 1000; j++)
			{
				//cols
				for (int k = 0; k < 1000; k++)
				{
					//3 channels
					for (int l = 0; l < 3; l++)
					{
						finalImg.at<Vec3b>(i / 5 * 1000 + j, i % 5 * 1000 + k)[l] = inProgressImg.at<Vec3b>(j, k)[l];
					}
				}
			}
		}
		imwrite(finalPath + "/" + extractLastFolderName(path) + " - FINAL.jpg", finalImg);
		auto stop = std::chrono::high_resolution_clock::now();
		timeDuration = stop - start;
		cout << "Folder " + extractLastFolderName(path) + " processed sucessfully in "<< timeDuration.count() << " s."<< endl;
		
	}
	catch (cv::Exception& e)
	{
		string message = getDate() + " " + getTime() + "Error occured in " + extractLastFolderName(path) + "folder. Skipping folder.";
		errorLog.push_back(message);
		cout << message << endl;
	}
	return timeDuration.count();
}
int main()
{
	cout << "Path to source folder: ";
	string path;
	std::getline(cin, path);
	cout << "Path to final folder: ";
	string finalPath;
	std::getline(cin, finalPath);
	int filesAmount;
	float totalTime = 0;
	vector<string>errorLog;
	vector<string>raport;
	if (path.size() == 0 || finalPath.size() == 0)
	{
		string message = getDate() + " " + getTime() + " Paths may be corrupted";
		errorLog.push_back(message);
		cout << message << endl;
	}
	else
	{
		//vector<string>insidePaths;
		//boost::filesystem::path the_path(path);
		//for (auto& p : boost::filesystem::directory_iterator(the_path))
		//{
		//	if (boost::filesystem::extension(p.path()) == ".jpg" || boost::filesystem::extension(p.path()) == ".png")
		//	{
		//		insidePaths.push_back(p.path().string());
		//	}
		//	//insidePaths.push_back(p.path().string());
		//	//cout << p << endl;
		//}
		//for (int i = 0; i < insidePaths.size(); i++)
		//{
		//	cout << insidePaths[i] << endl;
		//}
		filesAmount = returnAmountOfFilesInDirectory(path);
		if (filesAmount == 0)
		{
			//all folders
			vector<string>pathsToFoldersInside;
			returnPathsToFoldersInsideTheFolder(path, pathsToFoldersInside);
			for (int i = 0; i < pathsToFoldersInside.size(); i++)
			{
				if (returnAmountOfFilesInDirectory(pathsToFoldersInside[i]) == 0)
				{
					string message = getDate() + " " + getTime() + " Folder " + extractLastFolderName(pathsToFoldersInside[i]) + " is empty. Skipping folder.";
					raport.push_back(extractLastFolderName(pathsToFoldersInside[i]) + ": " + "0");
					errorLog.push_back(message);
					cout << message << endl;
					pathsToFoldersInside.erase(pathsToFoldersInside.begin() + i);
				}
			}
			if (pathsToFoldersInside.size() == 0)
			{
				string message = getDate() + " " + getTime() + " Folder does not contain any data to process.";
				errorLog.push_back(message);
				cout << message << endl;
			}
			else
			{
				for (int i = 0; i < pathsToFoldersInside.size(); i++)
				{
					raport.push_back(extractLastFolderName(pathsToFoldersInside[i]) + ": " + std::to_string(returnAmountOfFilesInDirectory(pathsToFoldersInside[i])));
					totalTime += processFilesFromDirectory(pathsToFoldersInside[i],returnAmountOfFilesInDirectory(pathsToFoldersInside[i]),finalPath, errorLog, raport);
				}
				cout << "DONE!" << endl;
				cout << "Total time: " << totalTime << " s." << endl;
			}
		}
		else
		{
			//only one folder
			raport.push_back(extractLastFolderName(path) + ": " + std::to_string(returnAmountOfFilesInDirectory(path)));
			totalTime += processFilesFromDirectory(path, filesAmount, finalPath, errorLog, raport);
			cout << "DONE!" << endl;
			cout << "Total time: " << totalTime << " s." << endl;
		}
	}
	//generateTextFile(finalPath, "Summary", raport);
	//generateTextFile(finalPath, "ErrorLog", errorLog);
	return 0;
}