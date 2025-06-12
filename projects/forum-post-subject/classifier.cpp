#include <iostream>
#include <map>
#include <string>
#include <set>
#include <fstream>
#include "csvstream.hpp"
#include <cmath>
using namespace std; 

set<string> unique_words(const string &str) {
    istringstream source(str);
    set<string> words;
    string word;
    while (source >> word) {
        words.insert(word);
    }
    return words;
}


class Classifier{
    private:
        //total number of training posts
        int total_posts = 0; 
        //total number of unique words
        set<string> vocabulary_size; 
        //For each label, the number of posts with that label
        map<string, int> num_posts_with_label; 
        //For each word, the number of posts in training set for which it appears
        map<string, int> total_word_occurrences; 
        //For each label, for each word, the number of posts in training set 
        //for which it appears
        map<string, map<string, int>> words_occurrence_count_per_label; 
        // store training examples for printing
        vector<pair<string, string>> training_data; 



    public:
        //default ctor
        Classifier(){}
        //default dtor
        //~Classifier();

        void process_training_data(const string& filename){
            csvstream csvin(filename);
            map<string, string> row;
            while(csvin >> row){
                string label = row["tag"];
                string post = row["content"];
                training_data.push_back({label, post}); //store for printing
                
                //stores only unique words
                set<string> words = unique_words(post);
                //increment total count of posts
                total_posts++;
                //increment count of posts with label
                num_posts_with_label[label]++;
                //iterate through unique words in post
                for(const string &word : words){
                    //add word to vocabulary    
                    vocabulary_size.insert(word);
                    //increment total word occurrences
                    total_word_occurrences[word]++;
                    //increment word occurrence count per label
                    words_occurrence_count_per_label[label][word]++;
                }
            }
        }

       map<string, double> find_log_prior() const{
              map<string, double> log_prior;
              for(const auto &label : num_posts_with_label){
                  string label_name = label.first;
                  int num_posts = label.second;
                  double prior = log(static_cast<double>(num_posts)/total_posts);
                //stores log prior probability of label
                  log_prior[label_name] = prior;

                  
              }
              return log_prior;
       }

       double find_log_likelihood(const string &word, const string &label) const{
           //numbe of times word appears in the given label
           int word_count_in_label = 0;
           //if word is a key for given label
           if (words_occurrence_count_per_label.count(label) > 0 
           && words_occurrence_count_per_label.at(label).count(word) > 0) {
                word_count_in_label = 
                words_occurrence_count_per_label.at(label).at(word);
           }
           //total number of posts with given label
           int label_count = num_posts_with_label.at(label);

            //if word not found in any posts with given label
            if(word_count_in_label == 0){
                //if word is found in any post
                if(total_word_occurrences.count(word)){
                    return log(static_cast<double>(total_word_occurrences.at(word))
                    /total_posts);
                }
                //if word is not found in any post
                else{
                    return log(1.0/total_posts);
                }
            }
            //use original if word is found in given label
            else{
                return log(static_cast<double>(word_count_in_label)/label_count);
            }
         }

            void print_classifier_params() const{
                cout << "training data:" << endl;
                for(const auto &data : training_data){
                    cout << "  label = " << data.first << ", content = " 
                    << data.second << endl;
                }
                cout << "trained on " << total_posts << " examples" << endl;
                cout << "vocabulary size = " << vocabulary_size.size() << endl;
                cout << endl; 
                
                
                map<string, double> log_prior = find_log_prior();
                cout << "classes:" << endl;
                for (const auto &label : num_posts_with_label) {
                    string label_name = label.first;
                    int num_posts = label.second;
                    double prior = log_prior[label_name];
                    cout << "  " << label_name << ", " << num_posts 
                    << " examples, log-prior = " << prior << endl;
                }       
                cout << "classifier parameters:" << endl;
                for (const auto &label: num_posts_with_label){
                    string label_name = label.first;
                    for (const auto &word: total_word_occurrences){
                        string word_name = word.first;
                        int count = 0;
                        if (words_occurrence_count_per_label.count(label_name) > 0 
                        && words_occurrence_count_per_label.at(label_name)
                        .count(word_name) > 0){
                            count = words_occurrence_count_per_label
                            .at(label_name).at(word_name);
                        }
                        if(count > 0){
                            double log_likelihood = 
                            find_log_likelihood(word_name, label_name);
                            cout << "  " << label_name 
                            << ":" << word_name << ", count = " << count 
                            << ", log-likelihood = " << log_likelihood << endl;
                        }
                        
                    }
                }
                cout << endl; 
    }

        void print(const string &filename, bool training_only){
            process_training_data(filename);
            if(training_only){
                print_classifier_params();
            }
            else{
                cout << "trained on " << total_posts << " examples" << endl;
                cout << endl; 
            }
        }

        string predict(const string &post) const{
            set<string> words = unique_words(post);
            map<string, double> log_prior = find_log_prior();
            map<string, double> label_scores;
            for(const auto &label : num_posts_with_label){
                string label_name = label.first;
                double score = log_prior.at(label_name);
                
                //make vector of alphabetical words to keep consistent order
                vector<string> alphabetical_words(words.begin(), words.end());
                sort(alphabetical_words.begin(), alphabetical_words.end());
                
                for(const string &word : alphabetical_words){
                    score += find_log_likelihood(word, label_name);
                }
                label_scores[label_name] = score;
            }

            string chosen_label;
            //set with very low score (negative infinity)
            double highest_score = -numeric_limits<double>::infinity();
            for(const auto &label : label_scores){
                //update best label based on highest log prob score
                if(label.second > highest_score){
                    chosen_label = label.first;
                    highest_score = label.second;
                }
                //if scores are tied, choose label earlier in alphabet
                else if(label.second == highest_score 
                    && label.first < chosen_label){
                    chosen_label = label.first; 
                }
            }
            return chosen_label;
    
        }

        // accuracy of classifier
        void accuracy(const string &filename) const{
            csvstream csvin(filename);
            map<string, string> row;
            int correct_predictions = 0;
            int total_predictions = 0;

            cout << "test data:" << endl;

            while(csvin >> row){
                string label = row["tag"];
                string post = row["content"];
                string prediction = predict(post);
                
                map<string, double> log_prior = find_log_prior();
                double log_prob_score = log_prior.at(prediction);
                for(const string &word : unique_words(post)){
                    log_prob_score += find_log_likelihood(word, prediction);
                }
                
                cout << "  correct = " << label 
                << ", predicted = " << prediction 
                << ", log-probability score = " << log_prob_score << endl;
                cout << "  content = " << post << endl << endl;

                if(prediction == label){
                    correct_predictions++;
                }
                total_predictions++;
            }
            cout << "performance: " << correct_predictions << " / " 
            << total_predictions << " posts predicted correctly" << endl;
        }
};

int main(int argc, char *argv[]){
    cout.precision(3);
    if(argc != 2 && argc != 3){
        cout << "Usage: classifier.exe TRAIN_FILE [TEST_FILE]" << endl;
        return 1;
    }
    ifstream train_file(argv[1]);
    if (!train_file.is_open()) {
        cout << "Error opening file: " << argv[1] << endl;
        return 1;
    }
    train_file.close();

    bool training_only = (argc == 2); 
    string training_data = argv[1];
    Classifier classifier;
    classifier.print(training_data, training_only);
    if (!training_only){
        ifstream test_file(argv[2]);
        if (!test_file.is_open()) {
            cout << "Error opening file: " << argv[2] << endl;
            return 1;
        }
        test_file.close();
        string test_data = argv[2];
        classifier.accuracy(test_data);
    }
    return 0;
}