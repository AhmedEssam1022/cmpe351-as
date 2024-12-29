int main(int argc, char* argv[]) {
    std::string inf, outf;
    int quantum = 0;
    
    for (int i = 1; i < argc; i += 2) {
        if (strcmp(argv[i], "-t") == 0)
            quantum = atoi(argv[i + 1]);
        else if (strcmp(argv[i], "-f") == 0)
            inf = argv[i + 1];
        else if (strcmp(argv[i], "-o") == 0)
            outf = argv[i + 1];
    }
    
    if (inf.empty() || outf.empty() || quantum == 0) {
        std::cout << "Usage: ./cpe351 -t <quantum> -f <input_file> -o <output_file>\n";
        return 1;
    }
    
    Scheduler s(inf, outf, quantum);
    s.read();
    s.write();
    
    return 0;
}
