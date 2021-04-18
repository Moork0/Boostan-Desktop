#ifndef SCORESHANDLER_H
#define SCORESHANDLER_H

#include "abstractxmldatahandler.h"
#include <QDebug>

class ScoresHandler : public AbstractXmlDataHandler
{
    Q_OBJECT

private:
    QVariantList _scores;
    QVariantMap _score_brief;
    // the semester number we wanna get information of
    QString _semester;
    QString _student_id;
    float _custom_average;
    bool  _need_custom_avg;

    // url of section we gonna send our requests.
    const inline static QString       _scores_url {QStringLiteral("/Forms/F1802_PROCESS_MNG_STDJAMEHMON/F1802_01_PROCESS_MNG_STDJAMEHMON_Dat.aspx?r=0.9638806400489983&fid=0;12310&b=0&l=0&&lastm=20180201081222&tck=")};


    // Forced implementation of AbstractXmlDataHandler pure function
    bool    getIsEmpty              () const override;

    //
    void    requestTokens();
    void    requestScores();

    // normalize the name and remove ugly chars
    void    normalizeName(QString& name);
    bool    extractScores(const QString& data);
    bool    extractBirefScores(const QString& data);
    QString extractXmlAttr(const QString& data, const QString& key, const bool search_at_start = true) const;

private slots:
    // parse the response to requestTokens()
    void            parseTokens(QNetworkReply& reply);
    void            parseScores(QNetworkReply& reply);

public:
    enum status
    {
        Passed,
        Failed,
        Deleted,
        Temporary,
        Undefined
    };
    Q_ENUM(status)

    ScoresHandler();

public slots:
    void start(const QString semester, const QString student_id);
    void getScoresOf(const int semester);
    QVariantList getScores() const;
    QVariantList getBriefScores() const;
};

#endif // SCORESHANDLER_H
