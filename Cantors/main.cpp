#include <iostream>
#include <sstream>
#include <fstream>
#include <map>

#define TEMP_LOG_FILE "log.log"
#define INPUT_FILE "CantorInput.config"

float m0St, m0Ed, m0Stp, m12St, m12E, m12Stp, a0St, a0Ed, a0Stp, tanbSt, tanbEd, tanbStp, higsValueToleranceRange = 0;
int signMu;
std::map<double,bool> higsValues;
std::map<std::string, bool> isHeaderWritten;
std::string softSussyExecutableFilePath,softSussyOutputFilePath, softsussyInputFilePath;
FILE * pLogFile;
inline void PauseConsole() { 	char a; 	std::cin >> a; }
inline void InitializeLogFile() { pLogFile = fopen(TEMP_LOG_FILE, "w"); if (pLogFile == NULL) throw std::runtime_error("Couldn't open the log file "); }
inline void CloseLogFile() { if(pLogFile != NULL)fclose(pLogFile); }
inline void WriteLog(const char* logEntry) { InitializeLogFile(); fprintf(pLogFile, logEntry); CloseLogFile(); }
inline void ReadInputFile(FILE* inputFile) {
	char* buffer = new char[1024];
	//read m0 header line
	std::fgets(buffer, 1024, inputFile);
	//read m0 st ed stp header line
	std::fgets(buffer, 1024, inputFile);
	//read m0 values
	std::fscanf(inputFile, "%f %f %f", &m0St, &m0Ed, &m0Stp);
	std::fgets(buffer, 1024, inputFile);
	//read a0 header line
	std::fgets(buffer, 1024, inputFile);
	//read a0 st ed stp header line
	std::fgets(buffer, 1024, inputFile);
	//read a0 values
	std::fscanf(inputFile, "%f %f %f", &a0St, &a0Ed, &a0Stp);
	std::fgets(buffer, 1024, inputFile);
	//read m12 header line
	std::fgets(buffer, 1024, inputFile);
	//read m12 st ed stp header line
	std::fgets(buffer, 1024, inputFile);
	//read m12 values
	std::fscanf(inputFile, "%f %f %f", &m12St, &m12E, &m12Stp);
	std::fgets(buffer, 1024, inputFile);
	//read tan(b) header line
	std::fgets(buffer, 1024, inputFile);
	//read tan(b) st ed stp header line
	std::fgets(buffer, 1024, inputFile);
	//read tan(b) values
	std::fscanf(inputFile, "%f %f %f", &tanbSt, &tanbEd, &tanbStp);
	std::fgets(buffer, 1024, inputFile);
	//Read Sign of mu header line
	std::fgets(buffer, 1024, inputFile);
	//Read Sign of mu value
	std::fscanf(inputFile, "%d", &signMu);
	std::fgets(buffer, 1024, inputFile);
	//Read the higs values header line 
	std::fgets(buffer, 1024, inputFile);
	//Read Higs Values line
	//Get the complete line
	std::string higsValuesLine = "";
	const char* higsValuesLineC;
	float tmp;
	char c = 0;
	do {
		c = ((char)std::fgetc(inputFile));
		if(c != '\n')
			higsValuesLine.push_back(c);
	} while (c != '\n');
	//Extract the higs values
	std::stringstream stream(higsValuesLine);
	while (stream.good()) {
		stream >> tmp;
		if (tmp > 0 && !higsValues.count(tmp))
			higsValues[tmp] = true;
	}
	//Read the higs values range tolerance header
	std::fgets(buffer, 1024, inputFile);
	//Read the higs values value
	std::fscanf(inputFile, "%f", &higsValueToleranceRange);
	std::fgets(buffer, 1024, inputFile);
	//Read the Softsussy input file path header
	std::fgets(buffer, 1024, inputFile);
	//Read the Softsussy input file path
	std::fscanf(inputFile, "%s", &softsussyInputFilePath);
	std::fgets(buffer, 1024, inputFile);
	//Read the Softsussy output file path header
	std::fgets(buffer, 1024, inputFile);
	//Read the Softsussy output file path
	std::fscanf(inputFile, "%s", &softSussyOutputFilePath);
	std::fgets(buffer, 1024, inputFile);
	//Read the Softsussy Executable file path header
	std::fgets(buffer, 1024, inputFile);
	//Read the Softsussy Executale file path
	std::fscanf(inputFile, "%s", &softSussyExecutableFilePath);
	delete[] buffer;
}
inline void WriteSoftSussyInputFile(FILE* softsussyInputFile,float m0,float m12,float a0,float tanb,float signmu) {
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
	fprintf(softsussyInputFile, "    1   %f	     # m0\n",&m0);
	fprintf(softsussyInputFile, "    2   %f	     # m12\n",&m12);
	fprintf(softsussyInputFile, "    3   %f	     # tan beta at MZ, in DRbar scheme, Feynman gauge\n",&tanb);
	fprintf(softsussyInputFile, "    4   %f	     # sign(mu)\n",signmu);
	fprintf(softsussyInputFile, "	 5   %f	     # A0\n",&a0);
	fprintf(softsussyInputFile, "Block SOFTSUSY               # Optional SOFTSUSY-specific parameters\n");
	fprintf(softsussyInputFile, "    1   1.000000000e-03      # Numerical precision: suggested range 10^(-3...-6)\n");
	fprintf(softsussyInputFile, "    2   0.000000000e+00	     # Quark mixing parameter: see manual\n");
	fprintf(softsussyInputFile, "    5   1.000000000e+00      # Include 2-loop scalar mass squared/trilinear RGEs\n");
}
inline float ReadHigsValueFromSoftsussyOutputFile(FILE* softsussyOutputFile) {
	std::string line;
	std::ifstream outputFileStream(softsussyOutputFile);
	int n;
	float higs;
	std::string t;
	while (std::getline(outputFileStream, line)) {
		if (line.find("# h0")) {
			sscanf(line.c_str(), "%d %f %s", &n, &higs, &t);
			break;
		}
	}
	return higs;
}
bool CheckHigsValues(double higsValue){
	if (higsValues.empty()) {
		std::fprintf(pLogFile, "");
		throw std::runtime_error("No Higs Boson to check!");
	}
	if (!higsValues.count(higsValue)) {
		auto st = *higsValues.lower_bound(higsValue - higsValueToleranceRange);
		auto ed = *higsValues.lower_bound(higsValue + higsValueToleranceRange);
		if (st.first >= higsValue - higsValueToleranceRange || ed.first <= higsValue + higsValueToleranceRange)
			return true;
		return false;
	}
	return true;
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
					//Open softsussy open file and get a pointer to it.
					FILE* softsussyInputFile = fopen(softsussyInputFilePath.c_str(), "w");
					if (softsussyInputFile != NULL) {
						//Write the input to softsussy Input file
						WriteSoftSussyInputFile(softsussyInputFile, m0, m12, a0, tanb, signMu);
						//Close the file and release the handle
						fclose(softsussyInputFile);
						//Execute the softsussy executable
						system(argument);
						//Read softsussy output result
						FILE* softsussyOutputFile = fopen(softSussyOutputFilePath.c_str(), "r");
						if (softsussyOutputFile != NULL) {
							float higs = ReadHigsValueFromSoftsussyOutputFile(softsussyOutputFile);
							if (CheckHigsValues(higs)) {
								std::string fileName = "Higs" + std::to_string(higs) + ".higs";
								FILE* higsFile = fopen(fileName.c_str(), "a");
								if (higsFile != NULL) {
									if (!isHeaderWritten[std::to_string(higs)]) {
										fprintf(higsFile, "m0(c1) m12(c2) TanBeta(c3) signMU(c4) A0(c5) h0(c6)\n");
										isHeaderWritten[std::to_string(higs)] = true;
									}
									fprintf(higsFile, "%f %f %f %d %f %f", &m0, &m12, &tanb, &signMu, &a0, &higs);
									fclose(higsFile);
								}
								else {
									WriteLog(("Failed to open an output file for higs value " + std::to_string(higs)).c_str());
									return 0;
								}
							}
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
