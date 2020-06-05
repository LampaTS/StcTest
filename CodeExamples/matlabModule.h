#ifndef MATLAB_MODULE_H
#define MATLAB_MODULE_H

#include <QObject>
#include <QMessageBox>
#include <QFile>
#include <QProcess>
#include <QDateTime>
#include <QStandardPaths>
#include <QFileInfo>
#include "headers/utils/blockStruct.h"
#include "headers/utils/settingsSingleton.h"
#include "headers/utils/utilities.h"

/* Класс для тестирования блоков обработки сигналов с радаров на автомобилях
 * Поступает группа блоков, checked - выбранные пользователем блоки, которые необходимо протестировать
 * unchecked - вспомогательные блоки, которые не тестируются, но необходимы для работы тестируемых блоков
 * Прогоняет входные данные через блоки и выводит логи в файл
 */

class matlabModule : public QObject
{
Q_OBJECT
    QVector<block> m_checked; // помеченные блоки
    QVector<block> m_unchecked; // остальные блоки
    QString m_inputPath; // путь до файла сданными
    QString m_outputPath; // путь к выходному файлу
    bool m_needDetails; // флаг, что нужна подробная информация
    scriptCaller* m_scriptCaller; // класс для вызова скриптов

public:
    matlabModule();
    ~matlabModule();
    matlabModule(const QVector<block> &m_unchecked, const QVector<block> &m_checked);
    bool run(); // выполнить анализ

public slots:
    void interrupt(); // прервать анализ (вызывается пользователем по кнопке)
};

#endif // MATLAB_MODULE_H
