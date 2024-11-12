#pragma once

#include <fstream>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>
#include <iostream>

using namespace std::string_literals; // makes visible operator""s

template <class... Ts> struct overloaded : Ts... {
    using Ts::operator()...;
};
template <class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

struct Config 
{
    
    using Node = std::variant<int, float, std::string>; // 输入参数可能是整数、小数、字符串
    std::unordered_map<std::string, Node> value
    {
        // 参数列表
        {"Input", ""s},
        {"Picking_templates", ""s},
        {"Template_dimensions", 2},
        {"Euler_angles_file", ""s},
        {"Pixel_size", -1.0f},
        {"Phi_step", -1.0f},
        {"n", -1.0f},
        {"Voltage", -1.0f},
        {"Cs", -1.0f}, // 单位mm
        {"Highest_resolution", -1.0f},
        {"Lowest_resolution", .0f},
        {"Diameter", -1.0f},
        // optional
        {"Score_threshold", 7.0f},
        {"Output", ""s},
        {"First_image", 0},
        {"Last_image", 0},
        {"GPU_ID", -1},
        {"Window_size", 320},
        {"Phase_flip", 0},
        {"Overlap", 0},
        {"Norm_type", 0},
        {"Invert", 0},
    };

    std::string & gets(const std::string & key) { return std::get<std::string>(value[key]); }
    const std::string & gets(const std::string & key) const { return std::get<std::string>(value.at(key)); }
    int & geti(const std::string & key) { return std::get<int>(value[key]); }
    const int & geti(const std::string & key) const { return std::get<int>(value.at(key)); }
    float & getf(const std::string & key) { return std::get<float>(value[key]); }
    const float & getf(const std::string & key) const { return std::get<float>(value.at(key)); }

    void print() 
    {
        for (auto&& [name, val] : value) 
        {
          std::cout << name << " = ";
          std::visit(overloaded{
                         [](auto arg) { std::cout << arg << ' '; },
                         [](float arg) { std::cout << std::fixed << arg << ' '; },
                         [](const std::string& arg) { std::cout << arg << ' '; },
                     },
                     val);
          std::cout << std::endl;
        }
    }

    // Parse from config file
    Config(const std::string & path);

    // Check all required parameters
    void checkRequestPara();
};

struct Parameters 
{
    float apix{}, kk{}, energy{}, cs{}, highres{}, lowres{}, d_m{}, thresh{};
    float lambda{}, dfu{}, dfv{}, ds{}, defocus{}, dfdiff{}, dfang{};
    float edge_half_width{4.0f}, ampconst{0.07f};
    float a{-10.81f}, b{1.f}, b2{0.32f}, bfactor{-18.17f}, bfactor2{-15.22f}, bfactor3{1.72f};

    Parameters() = default;
    Parameters(const Config & c) : apix(c.getf("Pixel_size")), kk(c.getf("n")), energy(c.getf("Voltage")), cs(c.getf("Cs")), highres(c.getf("Highest_resolution")), lowres(c.getf("Lowest_resolution")), d_m(c.getf("Diameter")), thresh(c.getf("Score_threshold")) {}
};

struct EulerData 
{
    std::vector<float> euler1, euler2, euler3;

    EulerData() = default;
    EulerData(const std::string & eulerf);
    size_t size() const { return euler1.size(); };
};

struct LST 
{
    struct Entry 
    {
        int unused; // 未使用的参数
        std::string rpath; // 文件路径
        double defocus; // 欠焦值
        double dfdiff; 
        double dfang;
    };

    static std::vector<Entry> load(const std::string & lst_path);
    static void print(const std::vector<Entry> & lst);
};