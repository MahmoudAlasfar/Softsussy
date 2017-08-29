#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <map>
#include <time.h>
#include <thread>
#include <cmath>

#define TEMP_LOG_FILE "log.log"
#define INPUT_FILE "CantorInput.config"

float m0St, m0Ed, m0Stp, m12St, m12E, m12Stp, a0St, a0Ed, a0Stp, tanbSt, tanbEd, tanbStp, higsValueToleranceRange = 0;
int signMu;
std::map<int, bool> higsValues;
std::map<int, bool> isHeaderWritten;
std::string softSussyExecutableFilePath, softSussyOutputFilePath, softsussyInputFilePath;
FILE * pLogFile;
std::string BuildStringFromBuffer(char* buffer, int bufferLength) {
	std::string str = "";
	bool isNonSpaceRead = false;
	for (int i = 0; i<bufferLength; i++) {
		if (buffer[i] == '\n' || buffer[i] == '\r')
			break;
		else if (buffer[i] != ' ')
			isNonSpaceRead = true;
		if ((buffer[i] == ' ' && isNonSpaceRead) || buffer[i] != ' ')
			str.push_back(buffer[i]);
	}
	return str;
}
inline void PauseConsole() { char a; 	std::cin >> a; }
inline void InitializeLogFile() { pLogFile = fopen(TEMP_LOG_FILE, "w"); if (pLogFile == NULL) throw std::bad_exception(); }
inline void CloseLogFile() { if (pLogFile != NULL)fclose(pLogFile); }
inline void WriteLog(const char* logEntry) { InitializeLogFile(); fprintf(pLogFile, logEntry); CloseLogFile(); }
inline void ReadInputFile(FILE* inputFile) {
	constexpr int maxBuffer = 4096;
	char* buffer = new char[maxBuffer];

	//read m0 header line
	std::fgets(buffer, maxBuffer, inputFile);
	//read m0 st ed stp header line
	std::fgets(buffer, maxBuffer, inputFile);
	//read m0 values
	std::fscanf(inputFile, "%f %f %f", &m0St, &m0Ed, &m0Stp);
	std::fgets(buffer, maxBuffer, inputFile);
	//read a0 header line
	std::fgets(buffer, maxBuffer, inputFile);
	//read a0 st ed stp header line
	std::fgets(buffer, maxBuffer, inputFile);
	//read a0 values
	std::fscanf(inputFile, "%f %f %f", &a0St, &a0Ed, &a0Stp);
	std::fgets(buffer, maxBuffer, inputFile);
	//read m12 header line
	std::fgets(buffer, maxBuffer, inputFile);
	//read m12 st ed stp header line
	std::fgets(buffer, maxBuffer, inputFile);
	//read m12 values
	std::fscanf(inputFile, "%f %f %f", &m12St, &m12E, &m12Stp);
	std::fgets(buffer, maxBuffer, inputFile);
	//read tan(b) header line
	std::fgets(buffer, maxBuffer, inputFile);
	//read tan(b) st ed stp header line
	std::fgets(buffer, maxBuffer, inputFile);
	//read tan(b) values
	std::fscanf(inputFile, "%f %f %f", &tanbSt, &tanbEd, &tanbStp);
	std::fgets(buffer, maxBuffer, inputFile);
	//Read Sign of mu header line
	std::fgets(buffer, maxBuffer, inputFile);
	//Read Sign of mu value
	std::fscanf(inputFile, "%d", &signMu);
	std::fgets(buffer, maxBuffer, inputFile);
	//Read the higs values header line
	std::fgets(buffer, maxBuffer, inputFile);
	//Read Higs Values line
	//Get the complete line
	std::string higsValuesLine = "";
	const char* higsValuesLineC;
	float tmp;
	char c = 0;
	do {
		c = ((char)std::fgetc(inputFile));
		if (c != '\n')
			higsValuesLine.push_back(c);
	} while (c != '\n');
	//Extract the higs values
	std::stringstream stream(higsValuesLine);
	while (stream.good()) {
		stream >> tmp;
		if (tmp > 0 && !higsValues.count(std::trunc(tmp)))
			higsValues[std::trunc(tmp)] = true;
	}
	//Read the higs values range tolerance header
	std::fgets(buffer, maxBuffer, inputFile);
	//Read the higs values value

	auto res = std::fscanf(inputFile, "%f", &higsValueToleranceRange);
	std::fgets(buffer, maxBuffer, inputFile);
	//Read the Softsussy input file path header
	std::fgets(buffer, maxBuffer, inputFile);
	//Read the Softsussy input file path

	std::fgets(buffer, maxBuffer, inputFile);
	softsussyInputFilePath = BuildStringFromBuffer(buffer, maxBuffer);
	//Read the Softsussy output file path header
	std::fgets(buffer, maxBuffer, inputFile);
	//Read the Softsussy output file path
	std::fgets(buffer, maxBuffer, inputFile);
	softSussyOutputFilePath = BuildStringFromBuffer(buffer, maxBuffer);
	//Read the Softsussy Executable file path header
	std::fgets(buffer, maxBuffer, inputFile);
	//Read the Softsussy Executale file path
	std::fgets(buffer, maxBuffer, inputFile);
	softSussyExecutableFilePath = BuildStringFromBuffer(buffer, maxBuffer);
	delete[] buffer;
}
inline void WriteSoftSussyInputFile(FILE* softsussyInputFile, float m0, float m12, float a0, float tanb, float signmu) {
	fprintf(softsussyInputFile, "# Example input in SLHA format, and suitable for input to\n");
	fprintf(softsussyInputFile, "# SOFTSUSY (v1.8 or higher): CMSSM10.1.1 input - see arXiv:1109.3859\n");
	fprintf(softsussyInputFile, "Block MODSEL		     # Select model\n");
	fprintf(softsussyInputFile, "    1    1		     # sugra\n");
	fprintf(softsussyInputFile, "Block SMINPUTS		     # Standard Model inputs\n");
	fprintf(softsussyInputFile, "    1	1.279340000e+02	     # alpha^(-1) SM MSbar(MZ)\n");
	fprintf(softsussyInputFile, "    2   1.166370000e-05	     # G_Fermi\n");
	fprintf(softsussyInputFile, "    3   1.172000000e-01	     # alpha_s(MZ) SM MSbar\n");
	fprintf(softsussyInputFile, "    4   9.118760000e+01	     # MZ(pole)\n");
	fprintf(softsussyInputFile, "    5	4.250000000e+00	     # mb(mb) SM MSbar\n");
	fprintf(softsussyInputFile, "    6   1.743000000e+02	     # mtop(pole)\n");
	fprintf(softsussyInputFile, "    7	1.777000000e+00	     # mtau(pole)\n");
	fprintf(softsussyInputFile, "Block MINPAR		     # Input parameters\n");
	fprintf(softsussyInputFile, "    1   %f	     # m0\n", m0);
	fprintf(softsussyInputFile, "    2   %f	     # m12\n", m12);
	fprintf(softsussyInputFile, "    3   %f	     # tan beta at MZ, in DRbar scheme, Feynman gauge\n", tanb);
	fprintf(softsussyInputFile, "    4   %f	     # sign(mu)\n", signmu);
	fprintf(softsussyInputFile, "	 5   %f	     # A0\n", a0);
	fprintf(softsussyInputFile, "Block SOFTSUSY               # Optional SOFTSUSY-specific parameters\n");
	fprintf(softsussyInputFile, "    1   1.000000000e-03      # Numerical precision: suggested range 10^(-3...-6)\n");
	fprintf(softsussyInputFile, "    2   0.000000000e+00	     # Quark mixing parameter: see manual\n");
	fprintf(softsussyInputFile, "    5   1.000000000e+00      # Include 2-loop scalar mass squared/trilinear RGEs\n");
}
inline float ReadHigsValueFromSoftsussyOutputFile(const char* filepath) {
	std::string line;
	std::ifstream outputFileStream(filepath);
	int n;
	float higs;
	bool isHigsFound = false;
	while (std::getline(outputFileStream, line)) {
		if (line.find("# h0") != std::string::npos) {
			sscanf(line.c_str(), "%d %f", &n, &higs);
			isHigsFound = true;
			break;
		}
	}
	if (!isHigsFound)
		return -1;
	return higs;
}
bool CheckHigsValues(float higsValue) {
	if (higsValues.empty()) {
		std::fprintf(pLogFile, "No Higs Boson to check!");
		throw std::bad_exception();
	}
	if (!higsValues.count(std::trunc(higsValue))) {
		for (auto hgs : higsValues) {
			float value = 0.0;
			if (higsValue > hgs.first)
				value = higsValue - hgs.first;
			else value = hgs.first - higsValue;
			if (value <= higsValueToleranceRange)
				return true;
		}
		return false;
	}
	return true;
}
float GetHigsValueForFileName(float higsValue) {
	float result = 0.0;
	if (!higsValues.count(std::trunc(higsValue))) {
		for (auto hgs : higsValues) {
			float value = 0.0;
			if (higsValue > hgs.first)
				value = higsValue - hgs.first;
			else value = hgs.first - higsValue;
			if (value <= higsValueToleranceRange) {
				result = hgs.first;
				break;
			}
		}
	}
	else result = higsValues[higsValue];
	return result;
}
int main() {
	//Reading the input file
	FILE* inputFile = fopen(INPUT_FILE, "r");
	if (inputFile != NULL) {
		ReadInputFile(inputFile);
		fclose(inputFile);
	}
	else {
		WriteLog("Couldn't read the input file");
		return 0;
	}
	const char* argument = (softSussyExecutableFilePath + " leshouches < " + softsussyInputFilePath + " > " + softSussyOutputFilePath).c_str();
	//Start the interations

	for (float m0 = m0St; m0 <= m0Ed; m0 += m0Stp) {
		for (float m12 = m12St; m12 <= m12E; m12 += m12Stp) {
			for (float a0 = a0St; a0 <= a0Ed; a0 += a0Stp) {
				for (float tanb = tanbSt; tanb <= tanbEd; tanb += tanbStp) {
					std::cout << "Trying m0=" << m0 << ", m12=" << m12 << ", a0=" << a0 << ", tanb=" << tanb << std::endl;
					//Open softsussy open file and get a pointer to it.
					FILE* softsussyInputFile = fopen(softsussyInputFilePath.c_str(), "w");
					if (softsussyInputFile != NULL) {
						//Write the input to softsussy Input file
						WriteSoftSussyInputFile(softsussyInputFile, m0, m12, a0, tanb, signMu);
						//Close the file and release the handle
						fclose(softsussyInputFile);
						//Execute the softsussy executable
						//incorrect generated output file name, TODO correct the file name for dynamic pathes
						//the pathes are fixed temprarely
						auto res = system("/home/mahmoud/softsusy-3.4.0/softpoint.x leshouches < /home/mahmoud/softsusy-3.4.0/Work/Inputtest_1 > /home/mahmoud/softsusy-3.4.0/Work/outtest_1");
						std::this_thread::sleep_for(std::chrono::milliseconds(10));
						//Read softsussy output result
						FILE* softsussyOutputFile = fopen(softSussyOutputFilePath.c_str(), "r");
						if (softsussyOutputFile != NULL) {
							float higs = ReadHigsValueFromSoftsussyOutputFile(softSussyOutputFilePath.c_str());
							if (higs == -1) {
								fclose(softsussyOutputFile);
								continue;
							}
							if (CheckHigsValues(higs)) {
								std::cout << "\tFound higs with the value " << higs << std::endl;
								auto higsForFileName = GetHigsValueForFileName(higs);
								std::string fileName = "Higs" + std::to_string(std::trunc(higsForFileName)) + ".higs";
								FILE* higsFile = fopen(fileName.c_str(), "a");
								if (higsFile != NULL) {
									if (!isHeaderWritten[std::trunc(higsForFileName)]) {
										fprintf(higsFile, "m0(c1) m12(c2) TanBeta(c3) signMU(c4) A0(c5) h0(c6)\n");
										isHeaderWritten[std::trunc(higsForFileName)] = true;
									}
									fprintf(higsFile, "%f %f %f %d %f %f\n", m0, m12, tanb, signMu, a0, higs);
									fclose(higsFile);
								}
								else {
									WriteLog(("Failed to open an output file for higs value " + std::to_string(higs)).c_str());
									return 0;
								}
							}
							else std::cout << "\tHigs value " << higs << " will not be selected." << std::endl;
							fclose(softsussyOutputFile);
						}
						else {
							WriteLog("Couldn't open the softsussy output file");
							return 0;
						}
					}
					else {
						WriteLog("Could't open the softsussy input file");
						return 0;
					}
				}
			}
		}
	}
	PauseConsole();
	return 0;
}
