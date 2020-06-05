#include "headers/modules/matlabModule.h"

matlabModule::matlabModule()
{

}

matlabModule::~matlabModule()
{
    delete m_scriptCaller;
}

matlabModule::matlabModule(const QVector<block> &unchecked_blocks, const QVector<block> &checked_blocks)
{
    // инициализируем векторы блоков
    for (auto block : unchecked_blocks)
        m_unchecked.append(block);
    for (auto block : checked_blocks)
        m_checked.append(block);

    // заполняем настройки
    m_inputPath = settingsSingleton::getInstance()->value("/settings/input_path").toString();
    m_outputPath = settingsSingleton::getInstance()->value("/settings/matlab_path").toString();
    m_needDetails = settingsSingleton::getInstance()->value("/settings/need_details").toBool();
    m_scriptCaller = new scriptCaller;
}

void matlabModule::interrupt()
{
    m_scriptCaller->setIsRunning(false);
}

bool matlabModule::run()
{
    // открываем и проверяем файл
    QFileInfo info(m_inputPath);
    if (!info.exists())
        throw std::runtime_error("Input file does not exist");

    m_scriptCaller->setIsRunning(true);

    if (m_checked.size() == 0)
        throw std::runtime_error("No checked blocks");

    // начинаем работу с временным файлом
    QString tmp_file_path = QStandardPaths::writableLocation(QStandardPaths::TempLocation)+"/matlab_tmp_file.csv";
    QFile::remove(tmp_file_path);
    QFile::copy(m_inputPath, tmp_file_path);

    // обрабатываем "невыбранные" блоки
    for (auto block : m_unchecked)
    {
        QVector<QString> commands;
        if (!checkPath(block.path))
            throw std::runtime_error("Wrong path to .exe file");
        commands.append(block.path);
        commands.append(tmp_file_path);
        commands.append(tmp_file_path);
        commands.append(block.NChan);
        commands.append(block.amount);
        commands.append(block.args);
        bool result = m_scriptCaller->callScript(commands);
        if (!result)
            throw std::runtime_error("Matlab script error");

        // если небходимы более подробные данные - выводим текущий этап в отдельный файл
        if (m_needDetails)
        {
            QString filename = generateFilename(block.name, "matlab_unchecked");
            QFile::remove(filename);
            QFile::copy(tmp_file_path, filename);
        }
    }

    // обрабатываем выбранные блоки
    for (auto block : m_checked)
    {
        QVector<QString> commands;
        if (!checkPath(block.path))
            throw std::runtime_error("Wrong path to .exe file");
        commands.append(block.path);
        commands.append(tmp_file_path);
        commands.append(tmp_file_path);
        commands.append(block.NChan);
        commands.append(block.amount);
        commands.append(block.args);
        bool result = m_scriptCaller->callScript(commands);
        if (!result)
            throw std::runtime_error("Matlab stopped");

        // если небходимы более подробные данные - выводим текущий этап в отдельный файл
        if (m_needDetails)
        {
            QString filename = generateFilename(block.name, "matlab_checked");
            QFile::remove(filename);
            QFile::copy(tmp_file_path, filename);
        }
    }

    // выводим итоги тестирования в файл
    QFile::remove(m_outputPath);
    QFile::copy(tmp_file_path, m_outputPath);
    QFile::remove(tmp_file_path);
    return true;
}
