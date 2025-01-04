#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <sstream>
#include <algorithm>

// Document structure to store document data
struct Document {
    int id;
    std::string text;
    float rank; // Rank is assigned during the search

    // Constructor for convenience
    Document(int id, const std::string& text, float rank)
        : id(id), text(text), rank(rank) {}
};

// Search engine class
class SearchEngine {
private:
    std::vector<Document> documents;
    std::unordered_map<std::string, std::vector<int>> invertedIndex;

public:
    void addDocument(int id, const std::string& text) {
        documents.push_back(Document(id, text, 0.0f)); // Add document
        std::istringstream stream(text);
        std::string word;
        while (stream >> word) {
            invertedIndex[word].push_back(id); // Build inverted index
        }
    }

    std::vector<Document> search(const std::string& query) {
        std::unordered_map<int, float> scores;
        std::istringstream stream(query);
        std::string word;

        while (stream >> word) {
            if (invertedIndex.find(word) != invertedIndex.end()) {
                for (int id : invertedIndex[word]) {
                    scores[id] += 1.0f; // Increment score for matching documents
                }
            }
        }

        std::vector<Document> results;
        for (auto& score : scores) {
            auto& doc = documents[score.first];
            doc.rank = score.second; // Update document rank
            results.push_back(doc);
        }

        // Sort results by rank in descending order
        std::sort(results.begin(), results.end(),
                  [](const Document& a, const Document& b) {
                      return a.rank > b.rank;
                  });

        return results;
    }
};

// Main function
int main() {
    SearchEngine engine;
    engine.addDocument(0, "Python crawler for NLP tasks");
    engine.addDocument(1, "C++ implementation of search algorithms");
    engine.addDocument(2, "NLP techniques and tools");

    auto results = engine.search("NLP");

    for (const auto& doc : results) {
        std::cout << "Document ID: " << doc.id
                  << ", Text: " << doc.text
                  << ", Rank: " << doc.rank << std::endl;
    }

    return 0;
}
