#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <array>
#include <unordered_set>
#include <unordered_map>
#include <regex>

std::regex FASTQ_HEADER_REGEX("CELL_([A-Z]+):UMI_([A-Z]+)");

bool get_read(std::istream& fastq_file, std::array<std::string, 4>& read) {
    std::string line;
    for (int i = 0; i < 4; i++) {
        if (!std::getline(fastq_file, line)) {
            return false;
        }
        read[i] = line;
    }
    return true;
}

std::array<std::string, 2> match_header(const std::string& header) {
    std::smatch match;
    std::regex_match(header, match, FASTQ_HEADER_REGEX);
    if (match.size() != 3) {
        std::cerr << "Could not match header: " << header << std::endl;
        exit(1);
    }
    std::array<std::string, 2> result;
    result[0] = match[1];
    result[1] = match[2];
    return result;
}

void process_fastq(
    std::istream& fastq_file,
    std::string& fastq_file_name,
    std::string& output_dir,
    std::unordered_set<std::string>& barcodes
) {
    std::array<std::string, 4> read;
    std::unordered_map<std::string, std::ofstream> output_files;
    size_t processed = 0;
    while (get_read(fastq_file, read)) {
        processed++;
        if (processed % 1000000 == 0) {
            std::cout << "Processed " << processed << " reads" << std::endl;
        }
        auto header_info = match_header(read[0]);
        auto& barcode = header_info[0];
        if (barcodes.find(barcode) == barcodes.end()) {
            continue;
        }
        if (output_files.find(barcode) == output_files.end()) {
            std::string output_file_name = output_dir + "/" + barcode + ".fastq";
            output_files[barcode] = std::ofstream(output_file_name);
        }
        auto& output_file = output_files[barcode];
        for (int i = 0; i < 4; i++) {
            output_file << read[i] << "\n";
        }
    }
    for (auto& output_file : output_files) {
        output_file.second.close();
    }
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " <fastq_file> <output_dir> <barcode_file>" << std::endl;
        return 1;
    }
    std::string fastq_file_name = argv[1];
    std::string output_dir = argv[2];
    std::string barcode_file_name = argv[3];
    std::unordered_set<std::string> barcodes;
    std::ifstream barcode_file(barcode_file_name);
    std::string barcode;
    while (std::getline(barcode_file, barcode)) {
        barcodes.insert(barcode);
    }
    if (fastq_file_name == "-") {
        process_fastq(std::cin, fastq_file_name, output_dir, barcodes);
    } else {
        std::ifstream fastq_file(fastq_file_name);
        process_fastq(fastq_file, fastq_file_name, output_dir, barcodes);
    }
}
