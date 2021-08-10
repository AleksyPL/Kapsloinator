#include "opencv2\opencv.hpp"
#include <iostream>
#include <algorithm>
#include <cmath>
#include <vector>
#include <chrono>
#include <ctime>
#include <fstream>
#include <filesystem>

using namespace cv;
namespace fs = std::filesystem;
using std::endl;
using std::cout;
using std::cin;
using std::string;
using std::vector;
using std::fstream;
using std::ios;

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
void returnPaths(string path, vector<fs::path>& insidePaths)
{
	for (auto& entry : fs::directory_iterator(path))
	{
		if (entry.is_directory())
		{
			returnPaths(entry.path().string(), insidePaths);
		}
		else if (entry.is_regular_file())
		{
			if (entry.path().extension() == ".jpg" || entry.path().extension() == ".png")
			{
				insidePaths.push_back(entry.path());
			}
		}
	}
}
void dividePathsByCountry(vector<fs::path> &insidePaths, vector<vector<fs::path>> &objects)
{
	vector<fs::path> dividedPaths;
	string lastCountry = "";
	string extractedCountry = "";
	for (int i = 0; i < insidePaths.size(); i++)
	{
		extractedCountry = extractLastFolderName(insidePaths[i].parent_path().string());
		if (lastCountry == "")
		{
			lastCountry = extractedCountry;
			dividedPaths.push_back(insidePaths[i]);
		}
		else if (lastCountry == extractedCountry)
		{
			dividedPaths.push_back(insidePaths[i]);
		}
		else
		{
			objects.push_back(dividedPaths);
			dividedPaths.clear();
			lastCountry = extractedCountry;
			dividedPaths.push_back(insidePaths[i]);
		}
	}
	if (std::find(objects.begin(), objects.end(), dividedPaths) == objects.end())
	{
		objects.push_back(dividedPaths);
	}
}
float processFiles(vector<string>insidePaths, string fileName, string finalPath, vector<string>& errorLog, vector<string>& raport)
{
	std::chrono::duration<float> timeDuration;
	auto start = std::chrono::high_resolution_clock::now();
	try
	{
		Mat errorImg = imread("./Error/Error.jpg", IMREAD_UNCHANGED);
		cout << fileName << ": Work in progress..." << endl;
		int targetHeight = 7;
		int targetWidth = 6;
		//if (insidePaths.size() > (targetHeight * targetWidth))
		//{
		//	int picturesPart = 0;
		//	int indexX = 0;
		//	int indexY = 0;
		//	cv::Mat finalImg(targetHeight * 1000, targetWidth * 1000, errorImg.type(), Scalar(0, 0, 0));
		//	//files
		//	for (int i = 0; i < insidePaths.size(); i++)
		//	{
		//		Mat inProgressImg = imread(insidePaths[i], IMREAD_UNCHANGED);
		//		if (inProgressImg.cols != 1000 || inProgressImg.rows != 1000)
		//		{
		//			Size size(1000, 1000);
		//			resize(inProgressImg, inProgressImg, size);
		//		}
		//		//rows
		//		for (int j = 0; j < 1000; j++)
		//		{
		//			//cols
		//			for (int k = 0; k < 1000; k++)
		//			{
		//				//3 channels
		//				for (int l = 0; l < 3; l++)
		//				{
		//					finalImg.at<Vec3b>(indexY * 1000 + j, indexX * 1000 + k)[l] = inProgressImg.at<Vec3b>(j, k)[l];
		//				}
		//			}
		//		}
		//		indexX++;
		//		if (indexX == targetWidth)
		//		{
		//			if (indexY == targetHeight - 1)
		//			{
		//				cv::imwrite(finalPath + "/" + fileName + " - FINAL(" + std::to_string(picturesPart) + ").jpg", finalImg);
		//				int remainingFiles = insidePaths.size() - i - 1;
		//				if (remainingFiles > targetHeight * targetWidth)
		//				{
		//					finalImg = Mat::zeros(targetHeight * 1000, targetWidth * 1000, finalImg.type());
		//				}
		//				else
		//				{
		//					int newSizeX = 0;
		//					int newSizeY = 0;
		//					//setting Y size
		//					if (remainingFiles % targetHeight != 0)
		//					{
		//						newSizeY = remainingFiles / targetHeight + 1;
		//					}
		//					else
		//					{
		//						newSizeY = remainingFiles / targetHeight;
		//					}
		//					//setting X size
		//					if (remainingFiles < targetWidth)
		//					{
		//						newSizeX = remainingFiles;
		//					}
		//					else
		//					{
		//						newSizeX = targetWidth;
		//					}
		//					finalImg = Mat::zeros(newSizeY * 1000, newSizeX * 1000, finalImg.type());
		//				}
		//				picturesPart++;
		//				indexX = 0;
		//				indexY = 0;
		//			}
		//			else
		//			{
		//				indexX = 0;
		//				indexY++;
		//			}
		//		}
		//	}
		//	cv::imwrite(finalPath + "/" + fileName + " - FINAL(" + std::to_string(picturesPart) + ").jpg", finalImg);
		//	auto stop = std::chrono::high_resolution_clock::now();
		//	timeDuration = stop - start;
		//	cout << "Folder " + fileName + " processed sucessfully in " << timeDuration.count() << " s." << endl;
		//	raport.push_back(fileName + ": " + std::to_string(insidePaths.size()));
		//}
		//else
		//{
		//	int indexX = 0;
		//	int indexY = 0;
		//	int pictureSizeX = 0;
		//	int pictureSizeY = 0;
		//	//setting Y size
		//	if (insidePaths.size() % targetWidth != 0)
		//	{
		//		pictureSizeY = insidePaths.size() / targetWidth + 1;
		//	}
		//	else
		//	{
		//		pictureSizeY = insidePaths.size() / targetWidth;
		//	}
		//	//setting X size
		//	if (insidePaths.size() < targetWidth)
		//	{
		//		pictureSizeX = insidePaths.size();
		//	}
		//	else
		//	{
		//		pictureSizeX = targetWidth;
		//	}
		//	cv::Mat finalImg(pictureSizeY * 1000, pictureSizeX * 1000, errorImg.type(), Scalar(0, 0, 0));
		//	//files
		//	for (int i = 0; i < insidePaths.size(); i++)
		//	{
		//		Mat inProgressImg = imread(insidePaths[i], IMREAD_UNCHANGED);
		//		if (inProgressImg.cols != 1000 || inProgressImg.rows != 1000)
		//		{
		//			Size size(1000, 1000);
		//			resize(inProgressImg, inProgressImg, size);
		//		}
		//		//rows
		//		for (int j = 0; j < 1000; j++)
		//		{
		//			//cols
		//			for (int k = 0; k < 1000; k++)
		//			{
		//				//3 channels
		//				for (int l = 0; l < 3; l++)
		//				{
		//					finalImg.at<Vec3b>(indexY * 1000 + j, indexX * 1000 + k)[l] = inProgressImg.at<Vec3b>(j, k)[l];
		//				}
		//			}
		//		}
		//		indexX++;
		//		if (indexX == targetWidth)
		//		{
		//			indexX = 0;
		//			indexY++;
		//		}
		//	}
		//	cv::imwrite(finalPath + "/" + fileName + " - FINAL.jpg", finalImg);
		//	auto stop = std::chrono::high_resolution_clock::now();
		//	timeDuration = stop - start;
		//	cout << "Folder " + fileName + " processed sucessfully in " << timeDuration.count() << " s." << endl;
		//	raport.push_back(fileName + ": " + std::to_string(insidePaths.size()));
		//}
	}
	catch (cv::Exception& e)
	{
		string message = getDate() + " " + getTime() + " - Error occured in " + fileName + " folder. Skipping folder.";
		errorLog.push_back(message);
		cout << message << endl;
		auto stop = std::chrono::high_resolution_clock::now();
		timeDuration = stop - start;
		return timeDuration.count();
	}
	return timeDuration.count();
}
int main()
{
	cout << "Kapsloinator by AleksyPL" << endl;
	//cout << "Path to source folder: ";
	string startPath = "E:\\Zdjêcia\\Kaplse\\Raw";
	//std::getline(cin, path);
	//cout << "Path to final folder: ";
	string finalPath = "E:\\Zdjêcia\\Kaplse\\Final";
	//std::getline(cin, finalPath);
	vector<string>errorLog;
	vector<string>raport;
	float totalTime = 0;
	int totalFiles = 0;
	if (startPath.size() == 0 || startPath.find('\\') == std::string::npos || finalPath.size() == 0 || finalPath.find('\\') == std::string::npos)
	{
		string message = getDate() + " " + getTime() + " Paths may be corrupted";
		errorLog.push_back(message);
		cout << message << endl;
	}
	else
	{
		vector<fs::path> rawPaths;
		vector<vector<fs::path>>objects;
		returnPaths(startPath, rawPaths);
		dividePathsByCountry(rawPaths, objects);
		for (int i = 0; i < objects.size(); i++)
		{
			vector <string> insidePaths;
			for (int j = 0; j < objects[i].size(); j++)
			{
				insidePaths.push_back(objects[i][j].string());
			}
			totalTime += processFiles(insidePaths, extractLastFolderName(objects[i][0].parent_path().string()), finalPath, errorLog, raport);
			totalFiles += insidePaths.size();
		}
		raport.push_back("");
		raport.push_back("Total: " + std::to_string(totalFiles));
		cout << "DONE!" << endl;
		cout << "Total time: " << totalTime << " s." << endl;
	}
	generateTextFile(finalPath, "SummaryReport", raport);
	if (errorLog.size() != 0)
	{
		generateTextFile(finalPath, "ErrorLog", errorLog);
	}
	return 0;
}