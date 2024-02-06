/**
 * @file main.cpp
 * @author Usam Tayyab (usamatayyab9@gmail.com)
 * @brief 
 * Compiled with QT 5.11.3 with gcc 9.4.0
 * 
 * This file is solution to "Problem 98. Fetching emails using IMAP"
 * mentioned in "Chapter 12 : Networking and Services" of the book:
 * - The Modern C++ Challenge by Marius Bancilla (available at amazon https://www.amazon.com/Modern-Challenge-programmer-real-world-problems/dp/1788993861)
 * 
 * Solution: 
 * Visit the following for IMAP commands:
 * -> https://www.atmail.com/blog/imap-commands/
 * -> https://www.atmail.com/blog/imap-101-manual-imap-sessions/
 * 
 * To implement this solution following classes and functions are defined.
 * - A `IMAP_Session` class which encapsulates a QSslSocket. This class is resposible for making connection with IMAP
 * server, sending request and reading response from IMAP server.
 * - A function `ListCommandResponseParser()` which parses output of LIST command.
 * - A function `IMAPResponseProcessor()` for processing IMAP server responses.
 * - A `Login()` function which initiates a login command to IMAP server. This must be first command that should be sent to
 *   server before making any other requests.
 * 
 * For printing list of folder
 * - A `PrintListOfFolders()` function which prints a list of folders on IMAP server.
 * 
 * For printing unseen mails from selected mail box:
 * - To print all unseen mails first we have to select the mailbox which is performed by `SelectMailBox()` function.
 * - Once the mailbox is selected now we have to extract unseem email from IMAP server which is done by calling
 *   `GetUnSeenMailsID()` which returns the vector if email IDs.
 * - Once we have all unseen mail IDs we can fetch email one by one using email ID. This is done by using `PrintMail()`
 *   function which fetches an email from its ID.
 * - All of the above three fcuntions are combined into a single function `PrintUnseenFrom()`.
 * 
 * - Once all the communication is done we must send a LOGOUT command to IMAP server which is done in `Logout()`
 * 
 * See function comments for IMAP command is generated and how its response it processed.
 * 
 * Driver code:
 * - Initialize IMAP_Session object
 * - Login into IMAP server
 * - Prints the list of folder
 * - Print all unseen mails from INBOX
 * - Initiate logout 
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include <QCoreApplication>
#include <QDebug>
#include <QTcpSocket>
#include <QSslSocket>
#include <QObject>
#include <QString>
#include <QByteArray>
#include <QRegularExpression>
#include <QTextStream>

#include <string_view>
#include <exception>
#include <vector>
#include <string>

using std::string;
using std::string_view;
using std::vector;

/**
 * @brief 
 * 
 */
class IMAP_Exception : public std::exception
{
    const char * message_;
public:
    IMAP_Exception(const char *msg) : message_{ msg }
    {
    }
    virtual const char * what() const noexcept
    {
        return message_;
    }
    ~IMAP_Exception(){}
};

/**
 * @brief 
 * 
 */
class IMAP_Session
{
public:
    IMAP_Session(const QString &host_name, const quint16 &port)
    {
        ssl_socket_.connectToHostEncrypted(host_name, port);
        if (false == ssl_socket_.waitForEncrypted(5000))
        {
            throw IMAP_Exception{ "Host Connection Timeout" };
        }
    }

    void SendRequest(string_view cmd)
    {
        static const auto kCommandFormat = QString{ "%1\r\n" };
        const auto kCompleteCommand      = kCommandFormat.arg(cmd.data()).toStdString();
        qDebug() << "command: " << kCompleteCommand.data();
        ssl_socket_.write(kCompleteCommand.data());
        if (false ==  ssl_socket_.waitForBytesWritten(-1))
        {
            throw IMAP_Exception{ "Request send timeout" };
        }
        ++command_sequence;
    }

    QString ReadResponse()
    {
        if (false == ssl_socket_.waitForReadyRead(-1)) {
            throw IMAP_Exception{ "Response read timeout" };
        }
        return ssl_socket_.readAll();
    }

private:
    QSslSocket ssl_socket_{};
    int command_sequence{ 1 };
};


void ListCommandResponseParser(const QString &response)
{
    QRegularExpression regex("\"(.+?)\"");
    QRegularExpressionMatchIterator matches = regex.globalMatch(response);
    while (matches.hasNext()) {
        QRegularExpressionMatch match = matches.next();
        QString folderName = match.captured(1);
        if ("/" != folderName)
        {
            qDebug() << "Folder Name: " << folderName;
        }
    }
}

/**
 * @brief Reads the response from IMAP server using param imap_session.
 * The response is read line by line. For each line that is read is passed to param line_processor.
 * So caller can define any way of processing it. Functions keeps on extracting as many lines as it
 * can until the line starts with param tag is received. See IMAP documenation for status that comes
 * with certain tags. Most common are OK, BAD.
 * 
 * @tparam LineProcessor 
 * @param imap_session 
 * @param tag 
 * @param line_processor 
 * @return QString 
 */
template<class LineProcessor>
QString IMAPResponseProcessor(IMAP_Session &imap_session, string_view tag, LineProcessor line_processor)
{
    auto is_response_complete = false;
    auto result_code          = QString{};
    while (false == is_response_complete)
    {
        const auto kResponse = imap_session.ReadResponse();
        const auto kChunks   = kResponse.split('\n');
        foreach (const auto &kLine, kChunks)
        {
            auto kTrimmed = kLine.trimmed();
            qDebug() << kTrimmed;
            line_processor(kTrimmed);
            if (kTrimmed.startsWith(tag.data()))
            {
                is_response_complete = true;
                result_code          = kTrimmed.split(' ')[1];//Inefficient, can be done with split
                qDebug() << "Result code: " << result_code;
            }
        }
    }
    return result_code;
}

/**
 * @brief IMAPResponseProcessor
 * @param imap_session
 * @param tag
 * @return
 */
QString IMAPResponseProcessor(IMAP_Session &imap_session, string_view tag)
{
    return IMAPResponseProcessor(imap_session, tag, [](QString){});
}

/**
 * @brief Performs a LOGIN request/response handshake with IMAP server.
 * @param imap_session
 * @param username
 * @param password
 * @return true - if OK is received
 *         false - if some other code is received
 */
bool Login(IMAP_Session &imap_session, string_view username, string_view password)
{
    constexpr auto kTag = string_view{ "A1" };
    auto login_command  = std::string{ kTag }.append( " LOGIN " ).append(username).append(" ").append(password);
    imap_session.SendRequest(login_command);
    const auto kResultCode = IMAPResponseProcessor(imap_session, kTag);
    return kResultCode == "OK";
}

/**
 * @brief Fetches list of folders from mailbox. param imap_session must be in state valid
 * i.e. connection already established and client logged in.
 * Send the command : A2 LIST "" *
 * Prcesses the response
 * @param imap_session
 */
void PrintListOfFolders(IMAP_Session &imap_session)
{
    constexpr auto kListCommandTag = string_view{ "A2" };
    const auto kListCommand = std::string{ kListCommandTag }.append(" LIST \"\" *");
    imap_session.SendRequest(kListCommand);
    const auto kResultCode = IMAPResponseProcessor(imap_session, kListCommandTag);
}

/**
 * @brief Selects the param mailbox on server. Mailbox is selected first before performing any operations
 * on it. The IMAP command that is send is : A3 EXAMINE INBOX
 * imap_session must be in a valid state i.e. initialized and client logged in.
 * @param imap_session
 * @param mailbox
 * @return true - if A3 OK is received
 *         false -  otherwise
 */
bool SelectMailBox(IMAP_Session &imap_session, string_view mailbox)
{
    constexpr auto kExamineCommandTag = string_view{ "A3" };
    auto mailbox_cmd = std::string{ kExamineCommandTag }.append( " EXAMINE " ).append(mailbox);
    imap_session.SendRequest(mailbox_cmd);
    const auto kResultCode = IMAPResponseProcessor(imap_session, kExamineCommandTag);
    return kResultCode == "OK";
}

/**
 * @brief  Fetches the list of all unseen email's ID. Before calling this function caller must ensure
 * that param imap_session is connected to IMAP server, logged in and have desired mail box selected.
 * Following command is sent to server : A4 SEARCH UNSEEN 
 * 
 * @param imap_session
 * @return vector<int> - Email IDs of all unseen emails.
 * @note Mailbox has to be selected prior calling to this function
 */
vector<int> GetUnSeenMailsID(IMAP_Session &imap_session)
{
    constexpr auto kUnseenMailCommandTag = string_view{ "A4" };
    imap_session.SendRequest(string{ kUnseenMailCommandTag }.append(" SEARCH UNSEEN"));
    auto is_response_complete = false;
    auto is_success           = false;
    auto unseen_message_uids  = vector<int>{};
    const auto kResultCode    = IMAPResponseProcessor(imap_session, kUnseenMailCommandTag,
        [&unseen_message_uids, &kUnseenMailCommandTag](QString str)
        {
            if (false == str.startsWith(kUnseenMailCommandTag.data()))
            {
                auto int_qregex = QRegularExpression{ "(\\d+)" };
                for (auto iter = int_qregex.globalMatch(str); iter.hasNext();)
                {
                    unseen_message_uids.push_back(iter.next().captured(1).toInt());
                }
            }
        }
    );
    if (kResultCode != "OK") { unseen_message_uids.clear(); }
    return unseen_message_uids;
}

/**
 * @brief Fetches an email from the server using the param msg_id.
 * Say msg_id == 1567, then command that is generated is:
 * A1567 FETCH (FLAGS BODY.PEEK[HEADER.FIELDS (FROM TO SUBJECT DATE)])
 * @param imap_session
 * @param msg_id
 */
void PrintMail(IMAP_Session &imap_session, const int &msg_id)
{
    const auto kMailCommandTag = string{ "A" }.append(std::to_string(msg_id));
    const auto kMailCommand    = string{ kMailCommandTag }
        .append(" FETCH ")
        .append(std::to_string(msg_id))
        .append(" (FLAGS BODY.PEEK[HEADER.FIELDS (FROM TO SUBJECT DATE)])");/*BODY.PEEK[TEXT] add in command to fetch body too*/
    imap_session.SendRequest(kMailCommand);
    const auto kResultCode = IMAPResponseProcessor(imap_session, kMailCommandTag);
    qDebug() << "MailID " << msg_id << ", status " << kResultCode;
}

/**
 * @brief Print all the unseen emails from param mailbox. To do so
 * - Mail box must be selected first
 * - All unseen email IDs has to fethced second
 * - Using email IDs all emails are to be extracted one by one.
 * @param imap_session
 * @param mailbox
 */
void PrintUnseenFrom(IMAP_Session &imap_session, string_view mailbox)
{
    if (SelectMailBox(imap_session, mailbox))
    {
        qDebug() << "Mailbox " << mailbox.data() << " selected";
        const auto kUnseenMailsID = GetUnSeenMailsID(imap_session);
        if (kUnseenMailsID.size() > 0)
        {
            qDebug() << "Total " << kUnseenMailsID.size() << " unread emails found";
            for (const auto &kMsgID : kUnseenMailsID)
            {
                PrintMail(imap_session, kMsgID);
            }
        }
    }
}

/**
 * @brief Peforms a LOGUT hanshake with the server.
 * Command : A6 LOGOUT
 * @param imap_session
 */
void Logout(IMAP_Session &imap_session)
{
    const auto kLogoutCommandTag = string_view{ "A6" };
    imap_session.SendRequest(string{kLogoutCommandTag}.append(" LOGOUT"));
    const auto kResultCode = IMAPResponseProcessor(imap_session, kLogoutCommandTag);
    qDebug() << kResultCode;
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    try
    {
        auto imap_session = IMAP_Session("imap.gmail.com", 993);
        qDebug() << "Connected";
        if (Login(imap_session, "your_email", "your password"))
        {
            qDebug() << "Login Successfull";
            PrintListOfFolders(imap_session);
            PrintUnseenFrom(imap_session, "INBOX");
        }
        Logout(imap_session);
    }
    catch (const IMAP_Exception &imap_ex)
    {
        qDebug() << imap_ex.what();
    }
    return a.exec();
}
