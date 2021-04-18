#ifndef SCORESHANDLER_H
#define SCORESHANDLER_H

/*
    * Class: ScoresHandler
    * Files: scoreshandler.h, scoreshandler.cpp
    * The task of this class is:
    * 1- Retrieve scores for a specific semester from Golestan
    * 2- re-calculate scores average if we have some temporary scores (because Golestan would not count them)
*/

#include "abstractxmldatahandler.h"
#include <QDebug>

class ScoresHandler : public AbstractXmlDataHandler
{
    Q_OBJECT

private:
    // containers
    QVariantList _scores;
    QVariantMap _score_brief;

    // the semester number we wanna get information of
    QString _semester;
    QString _student_id;
    // re-calculated average stores here
    float _custom_average;
    // flag which determine the need of re-calculating of averages
    bool  _need_custom_avg;

    // url of section we gonna send our requests.
    const inline static QString       _scores_url {QStringLiteral("/Forms/F1802_PROCESS_MNG_STDJAMEHMON/F1802_01_PROCESS_MNG_STDJAMEHMON_Dat.aspx?r=0.9638806400489983&fid=0;12310&b=0&l=0&&lastm=20180201081222&tck=")};


    // Forced implementation of AbstractXmlDataHandler pure function
    bool    getIsEmpty              () const override;

    // requests to Golestan
    void    requestTokens();
    void    requestScores();

    // normalize the name and remove ugly chars
    void    normalizeName(QString& name);
    bool    extractScores(const QString& data);
    bool    extractBirefScores(const QString& data);
    // extract the value of 'key' from Golestan-provided XML data which have stored in 'data'
    QString extractXmlAttr(const QString& data, const QString& key, const bool start_from_first = true) const;

private slots:
    // parse the response to requestTokens()
    void            parseTokens(QNetworkReply& reply);
    // parse the response to requestScores()
    void            parseScores(QNetworkReply& reply);

public:
    // Course status
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
    // Initialize the request-sequence
    void start(const QString semester, const QString student_id);
    // get scores of semester 'semester'
    void getScoresOf(const QString semester);
    // return containers
    QVariantList getScores() const;
    QVariantList getBriefScores() const;
};

#endif // SCORESHANDLER_H
