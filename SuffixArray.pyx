# distutils: language = c++

cimport cython
from libcpp.vector cimport vector
from libcpp.string cimport string
from libcpp.map cimport pair

cdef extern from "cpp_code/suffix_array.h":
    cdef cppclass suffix_array:
        suffix_array() except +
        suffix_array(string s) except +
        int size() except +
        int get_id(int suf_rank) except +
        string get_suf(int suf_rank) except +
        int get_rank(int suf_id) except +
        int get_count(const string &t) except +
        vector[pair[string, double]] get_prob(const string &t) except +
        double get_branch_entropy(const string &t) except +
        double get_mutual_information(const string &t) except +

cdef class SuffixArray:
    cdef suffix_array* sa

    def __cinit__(self, file_paths = None, text = None):
        if not file_paths and not text:
            raise ValueError("Either file_paths or text must be provided.")

        if isinstance(file_paths, str):
            file_paths = [file_paths]
        if file_paths:
            text = ''
            for file_path in file_paths:
                with open(file_path, 'r', encoding='utf-8') as file:
                    text += file.read() + "\n"
        self.sa = new suffix_array(text.encode('utf-8'))


    def __dealloc__(self):
        del self.sa

    def size(self):
        return self.sa.size()

    def get_id(self, suf_rank):
        return self.sa.get_id(suf_rank)

    def get_suf(self, suf_rank):
        cdef string cpp_suf = self.sa.get_suf(suf_rank)
        try:
            return cpp_suf.decode('utf-8')
        except UnicodeDecodeError:
            return "<INVALID UTF-8 DATA>"

    def get_rank(self, suf_id):
        return self.sa.get_rank(suf_id)
    
    def get_count(self, pattern):
        return self.sa.get_count(pattern.encode('utf-8'))

    def get_prob(self, prompt):
        cdef vector[pair[string, double]] cpp_prob = self.sa.get_prob(prompt.encode('utf-8'))
        prob = []
        for item in cpp_prob:
            prob.append({'token': item.first.decode('utf-8'), 'prob': item.second})
        return prob
        
    def get_branch_entropy(self, prompt):
        return self.sa.get_branch_entropy(prompt.encode('utf-8'))

    def get_mutual_information(self, prompt):
        return self.sa.get_mutual_information(prompt.encode('utf-8'))
