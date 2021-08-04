#include "opencv2\opencv.hpp"
//#include <boost/filesystem.hpp>
#include <iostream>
#include <algorithm>
#include <cmath>
#include <vector>
#include <chrono>
#include <ctime>
#include <fstream>
#include <filesystem>

using namespace cv;
//using namespace boost::filesystem;
using std::endl;
using std::cout;
using std::cin;
using std::string;
using std::vector;
using std::fstream;
using std::ios;
namespace fs = std::filesystem;

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
	int i = 0;
	for (auto& entry : fs::directory_iterator(pathString))
	{
		if (entry.is_regular_file())
		{
			i++;
		}
	}
	return i;
	/*boost::filesystem::path the_path(pathString);
	int cnt = std::count_if(directory_iterator(the_path), directory_iterator(), static_cast<bool(*)(const boost::filesystem::path&)>(is_regular_file));
	return cnt;*/
}
void returnPathsToFoldersInsideTheFolder(string sourcePath, vector<string>&insidePaths)
{
	for (auto& entry : fs::directory_iterator(sourcePath))
	{
		if (entry.is_regular_file())
		{
			insidePaths.push_back(entry.path().string());
		}
	}
	/*boost::filesystem::path the_path(sourcePath);
	for (auto& p : boost::filesystem::directory_iterator(the_path))
	{
		insidePaths.push_back(p.path().string());
	}*/
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
		if (i == vectorFile.size() - 1)
		{
			file << vectorFile[i];
		}
		else
		{
			file << vectorFile[i] << endl;
		}
		
	}
	file.close();
}
float processFilesFromDirectory(string path, string finalPath, vector<string>&errorLog, vector<string>&raport)
{
	std::chrono::duration<float> timeDuration;
	auto start = std::chrono::high_resolution_clock::now();
	try
	{
		Mat errorImg = imread("./Error/Error.jpg", IMREAD_UNCHANGED);
		cout << extractLastFolderName(path) << ": Work in progress..." << endl;
		vector<string>insidePaths;
		for (auto& entry : fs::directory_iterator(path))
		{
			if (entry.path().extension() == ".jpg" || entry.path().extension() == ".png")
			{
				insidePaths.push_back(entry.path().string());
			}
		}
		/*boost::filesystem::path the_path(path);
		for (auto& p : boost::filesystem::directory_iterator(the_path))
		{
			if (boost::filesystem::extension(p.path()) == ".jpg" || boost::filesystem::extension(p.path()) == ".png")
			{
				insidePaths.push_back(p.path().string());
			}
		}*/
		if (insidePaths.size() == 0)
		{
			string message = getDate() + " " + getTime() + " Folder " + extractLastFolderName(path) + " is empty. Skipping folder.";
			raport.push_back(extractLastFolderName(path) + ": " + "0");
			errorLog.push_back(message);
			auto stop = std::chrono::high_resolution_clock::now();
			timeDuration = stop - start;
			return timeDuration.count();
		}
		else
		{
			//int rowsInPhoto = 0;
			//int colsInPhoto = 0;
			int targetHeight = 7;
			int targetWidth = 6;
			
			if (insidePaths.size() > (targetHeight * targetWidth))
			{
				int picturesPart = 0;
				int indexX = 0;
				int indexY = 0;
				cv::Mat finalImg(targetHeight * 1000, targetWidth * 1000, errorImg.type(), Scalar(0, 0, 0));
				//files
				for (int i = 0; i < insidePaths.size(); i++)
				{
					Mat inProgressImg = imread(insidePaths[i], IMREAD_UNCHANGED);
					if (inProgressImg.cols == 0 || inProgressImg.rows == 0)
					{
						string message = getDate() + " " + getTime() + " Picture nr. " + std::to_string(i) + " in " + extractLastFolderName(path) + " cannot be loaded. Skipping picture.";
						errorLog.push_back(message);
						cout << message << endl;
						inProgressImg = errorImg;
					}
					else if (inProgressImg.cols != 1000 || inProgressImg.rows != 1000)
					{
						Size size(1000, 1000);
						resize(inProgressImg, inProgressImg, size);
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
								finalImg.at<Vec3b>(indexY * 1000 + j, indexX * 1000 + k)[l] = inProgressImg.at<Vec3b>(j, k)[l];
							}
						}
					}
					indexX++;
					if (indexX == targetWidth)
					{
						if (indexY == targetHeight - 1)
						{
							cv::imwrite(finalPath + "/" + extractLastFolderName(path) + " - FINAL(" + std::to_string(picturesPart) + ").jpg", finalImg);
							int remainingFiles = insidePaths.size() - i - 1;
							if (remainingFiles > targetHeight * targetWidth)
							{
								finalImg = Mat::zeros(targetHeight * 1000, targetWidth * 1000, finalImg.type());
							}
							else
							{
								int newSizeX = 0;
								int newSizeY = 0;
								//setting Y size
								if (remainingFiles % targetHeight != 0)
								{
									newSizeY = remainingFiles / targetHeight + 1;
								}
								else
								{
									newSizeY = remainingFiles / targetHeight;
								}
								//setting X size
								if (remainingFiles < targetWidth)
								{
									newSizeX = remainingFiles;
								}
								else
								{
									newSizeX = targetWidth;
								}
								finalImg = Mat::zeros(newSizeY * 1000, newSizeX * 1000, finalImg.type());
							}
							picturesPart++;
							indexX = 0;
							indexY = 0;
						}
						else
						{
							indexX = 0;
							indexY++;
						}
					}
				}
				cv::imwrite(finalPath + "/" + extractLastFolderName(path) + " - FINAL(" + std::to_string(picturesPart) + ").jpg", finalImg);
				auto stop = std::chrono::high_resolution_clock::now();
				timeDuration = stop - start;
				cout << "Folder " + extractLastFolderName(path) + " processed sucessfully in " << timeDuration.count() << " s." << endl;
			}
			else
			{
				int indexX = 0;
				int indexY = 0;
				int pictureSizeX = 0;
				int pictureSizeY = 0;
				//setting Y size
				if (insidePaths.size() % targetWidth != 0)
				{
					pictureSizeY = insidePaths.size() / targetWidth + 1;
				}
				else
				{
					pictureSizeY = insidePaths.size() / targetWidth;
				}
				//setting X size
				if (insidePaths.size() < targetWidth)
				{
					pictureSizeX = insidePaths.size();
				}
				else
				{
					pictureSizeX = targetWidth;
				}
				cv::Mat finalImg(pictureSizeY * 1000, pictureSizeX * 1000, errorImg.type(), Scalar(0, 0, 0));
				//files
				for (int i = 0; i < insidePaths.size(); i++)
				{
					Mat inProgressImg = imread(insidePaths[i], IMREAD_UNCHANGED);
					if (inProgressImg.cols == 0 || inProgressImg.rows == 0)
					{
						string message = getDate() + " " + getTime() + " Picture nr. " + std::to_string(i) + " in " + extractLastFolderName(path) + " cannot be loaded. Skipping picture.";
						errorLog.push_back(message);
						cout << message << endl;
						inProgressImg = errorImg;
					}
					else if (inProgressImg.cols != 1000 || inProgressImg.rows != 1000)
					{
						Size size(1000, 1000);
						resize(inProgressImg, inProgressImg, size);
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
								finalImg.at<Vec3b>(indexY * 1000 + j, indexX * 1000 + k)[l] = inProgressImg.at<Vec3b>(j, k)[l];
							}
						}
					}
					indexX++;
					if (indexX == targetWidth)
					{
						indexX = 0;
						indexY++;
					}
				}
				cv::imwrite(finalPath + "/" + extractLastFolderName(path) + " - FINAL.jpg", finalImg);
				auto stop = std::chrono::high_resolution_clock::now();
				timeDuration = stop - start;
				cout << "Folder " + extractLastFolderName(path) + " processed sucessfully in " << timeDuration.count() << " s." << endl;
			}
		}
	}
	catch (cv::Exception& e)
	{
		string message = getDate() + " " + getTime() + " - Error occured in " + extractLastFolderName(path) + " folder. Skipping folder.";
		errorLog.push_back(message);
		cout << message << endl;
		auto stop = std::chrono::high_resolution_clock::now();
		timeDuration = stop - start;
		return timeDuration.count();
	}
	return timeDuration.count();
}
//void checkPathForSlashes(string& path)
//{
//	int stringSize = path.length();
//	for (int i = 0; i < stringSize; i++)
//	{
//		if ((i - 1) >= 0 && (i + 1) <= stringSize && path[i] == '\\' && path[i - 1] != '\\' && path[i + 1] != '\\')
//		{
//			string tempBeforeSlash= "";
//			string tempAfterSlash = "";
//			for (int j = 0; j < i; j++)
//			{
//				tempBeforeSlash += path[i];
//			}
//			for (int j = i; j < stringSize; j++)
//			{
//				tempAfterSlash += path[i];
//			}
//			path = tempBeforeSlash + '\\' + tempAfterSlash;
//			stringSize++;
//		}
//	}
//}
int main()
{
	cout << "Kapsloinator by AleksyPL" << endl;
	cout << "Path to source folder: ";
	string path = "E:\\Zdjêcia\\Kaplse\\Raw\\Egipt";
	returnAmountOfFilesInDirectory(path);
	//std::getline(cin, path);
	//checkPathForSlashes(path);
	cout << "Path to final folder: ";
	string finalPath = "E:\\Zdjêcia\\Kaplse\\Final";
	//std::getline(cin, finalPath);
	int filesAmount;
	float totalTime = 0;
	int totalFiles = 0;
	vector<string>errorLog;
	vector<string>raport;
	if (path.size() == 0 || path.find('\\') == std::string::npos || finalPath.size() == 0 || finalPath.find('\\') == std::string::npos)
	{
		string message = getDate() + " " + getTime() + " Paths may be corrupted";
		errorLog.push_back(message);
		cout << message << endl;
	}
	else
	{
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
					totalTime += processFilesFromDirectory(pathsToFoldersInside[i],finalPath, errorLog, raport);
					totalFiles += returnAmountOfFilesInDirectory(pathsToFoldersInside[i]);
				}
				raport.push_back("");
				raport.push_back("Total: " + std::to_string(totalFiles));
				cout << "DONE!" << endl;
				cout << "Total time: " << totalTime << " s." << endl;
			}
		}
		else
		{
			//only one folder
			raport.push_back(extractLastFolderName(path) + ": " + std::to_string(returnAmountOfFilesInDirectory(path)));
			totalFiles += returnAmountOfFilesInDirectory(path);
			raport.push_back("");
			raport.push_back("Total: " + std::to_string(totalFiles));
			totalTime += processFilesFromDirectory(path, finalPath, errorLog, raport);
			cout << "DONE!" << endl;
			cout << "Total time: " << totalTime << " s." << endl;
		}
	}
	generateTextFile(finalPath, "SummaryReport", raport);
	if (errorLog.size() != 0)
	{
		generateTextFile(finalPath, "ErrorLog", errorLog);
	}
	return 0;
}