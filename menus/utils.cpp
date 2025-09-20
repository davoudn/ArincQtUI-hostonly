#include "utils.h"


namespace utils {

std::vector<uint8_t> getDecimals(uint32_t value, uint8_t sidigits){
    std::vector<uint8_t> decimals;
    uint8_t _t{0};
    uint32_t _value{value};

    for (uint8_t x = 0; x < sidigits; x++) {
        uint32_t magnitude = std::pow(10, sidigits - x -1);
        _t = (uint32_t)(_value/magnitude);
        decimals.push_back(_t);
        _value = _value - _t * magnitude;
    }
    return decimals;
}


str_t ReadFile(str_t qfilename){
    std::string filename = qfilename.toStdString();
    std::ifstream file(filename);
    // Check if the file opened successfully
    if (!file.is_open()) {
        qDebug() << "Label::ReadFile: Failed to open file: " << qfilename;
        exit(1);
    }
    // Read the entire file into a string
    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    // Close the file
    file.close();

    return str_t(content.c_str());
}

bool writeFile(str_t qfilename, str_t data){
    std::string filename = qfilename.toStdString();
    std::ofstream file(filename);
    if (!file.is_open()){
        qInfo() << "writeFile : filename " <<  qfilename << " does not exist!";
        return false;
    }
    file.write(data.toStdString().c_str(), data.size());
    file.close();
    return true;
}

jsonobj_t GetJsonObj(str_t _jsonfilename){
    str_t  jsonstr =  ReadFile(_jsonfilename);
    jsondoc_t jdoc = jsondoc_t::fromJson(jsonstr.toUtf8());
    return jdoc.object();
}

bool dumpJsonObj(str_t _jsonfilename, jsonobj_t _jobj){
    jsondoc_t jdoc (_jobj);
    if(writeFile(_jsonfilename, jdoc.toJson(QJsonDocument::Indented))) {
        return true;
    }
    return false;
}


QString openFileDialogForOpening(QWidget* parent, QString startingDir) {
    QString fileName = QFileDialog::getOpenFileName(
        parent,
        "Select a file",
        startingDir,         // Starting directory
        "Text Files (*.json)"
    );
//         "All Files (*.*);;Text Files (*.txt);;Images (*.png *.jpg)"

    if (!fileName.isEmpty()) {
        qDebug() << "Selected file:" << fileName;
    } else {
        qDebug() << "No file selected.";
    }

    return fileName;
}

QString openFileDialogForSaving(QWidget* parent, QString startingDir) {
    QString fileName = QFileDialog::getSaveFileName(
        parent,
        "Create Text File",
        startingDir,         // Starting directory
        "Text Files (*.json)"
    );
//         "All Files (*.*);;Text Files (*.txt);;Images (*.png *.jpg)"

    if (!fileName.isEmpty()) {
        qDebug() << "Selected file:" << fileName;
    } else {
        qDebug() << "No file selected.";
    }

    return fileName;
}

//

using pair_t = std::pair<uint8_t,uint8_t>;


template <>
MyBitset MyBitset::init<uint32_t>(uint32_t t, uint8_t _format){
    bIfInitialized = false;
    data.clear();
    uint32_t _t{t};
    while (_t>0){
        data.push_back(_t - 2 * (_t/2));
        _t = _t/2;
    }
    if (_format > 0){
        if (_format>data.size()){
            for (uint8_t i=0; i < _format-data.size();i++){
                data.push_back(0);
            }
        }
        if (_format<data.size()){
            data.resize(_format);
        }
    }

    bIfInitialized = true;
    return *this;
}

template <>
MyBitset MyBitset::init<std::string>(std::string t, uint8_t _format){
    bIfInitialized = false;
    data.clear();
    for(auto& x: t){
        if ( x!='0' && x!='1'){
            return *this;
        }
    }
    for(auto& x: t){
        if (x=='0'){
            data.push_back(0);
        }
        if (x=='1'){
            data.push_back(1);
        }
    }
    if (_format > 0){
        if (_format>data.size()){
            for (uint8_t i=0; i < _format-data.size();i++){
                data.push_back(0);
            }
        }
        if (_format<data.size()){
            data.resize(_format);
        }
    }
    bIfInitialized = true;
    return  *this;
}


}
