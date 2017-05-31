#include "stringgenerator.h"

/**
 * @brief StringGenerator::StringGenerator
 * html字符串生成
 */
StringGenerator::StringGenerator()
{

}

QString StringGenerator::getDashedLine()
{
    return TE(QString("#787775"),QString("consolas"),QString("----------------------------"));
}

QString StringGenerator::getString(QMap<QString, QString> &messageMap, bool textOnly, bool showName, bool showGrade)
{
    QString str = "";
    QString font_color_blue = "#3B91C5";
    QString font_color_red = "#E34945";
    QString font_color_black = "#454545";
    QString font_MS = "Microsoft YaHei";
    QString font_Con = "consolas";

    FF func;
    if (textOnly)
    {
        func = TT;
    }
    else
    {
        func = TE;
    }
    if(messageMap["type"] == "chatmsg") //聊天
    {
        QString name = func(font_color_blue,font_MS,messageMap["nn"]);
        QString grade = func(font_color_red,font_Con,"[lv."+messageMap["level"]+"]");
        QString txt = func(font_color_black,font_MS, messageMap["txt"]);

        if (showName)
        {
            str += name;
        }
        if (showGrade)
        {
            str += grade;
        }
        if (!str.isEmpty())
        {
            str += ":";
        }
        str += txt;
//        str = QString("%1 %2 %3 %4").arg(func(font_color_blue,font_MS,messageMap["nn"]))
//                .arg(func(font_color_red,font_Con,"[lv."+messageMap["level"]+"]"))
//                .arg(func(font_color_blue,font_MS,":"))
//                .arg(func(font_color_black,font_MS, messageMap["txt"]));
    }
    else if(messageMap["type"] == "onlinegift") //领取鱼丸暴击
    {
        str = QString("");
    }
    else if(messageMap["type"] == "dgb") //赠送礼物
    {
        /*-----------------------------------------------------------------------------------
         * gfid    |  50   |     53        |  57       |  52     |  54         |     59     |
         * ---------------------------------------------------------------------------------
         * 表示内容| 100鱼丸|我爱你(520鱼丸)|赞(0.1鱼翅)|666(6鱼翅)|飞机(100鱼翅)|火箭(500鱼翅)|
         * ----------------------------------------------------------------------------------
         *
         */

        int gfid = messageMap["gfid"].toInt();
        QString gfid_str = "";
        switch (gfid) {
        case 50:
            gfid_str = QStringLiteral("100鱼丸");
            break;
        case 52:
            gfid_str = QStringLiteral("6鱼翅(666)");
            break;
        case 53:
            gfid_str = QStringLiteral("520鱼丸(我爱你)");
            break;
        case 54:
            gfid_str = QStringLiteral("100鱼翅(飞机)");
            break;
        case 57:
            gfid_str = QStringLiteral("0.1鱼翅(赞)");
            break;
        case 59:
            gfid_str = QStringLiteral("500鱼翅(火箭)");
            break;
        default:
            gfid_str = QStringLiteral("什么鬼?(礼物)");
            break;
        }
        str = QString("%1 %2 %3").arg(func(font_color_blue,font_MS,messageMap["nn"]))
                .arg(func(font_color_black,font_MS, QStringLiteral("赠送给主播")))
                .arg(func(font_color_red,font_Con+","+font_MS,gfid_str));
    }
    else if(messageMap["type"] == "uenter") //特殊身份用户进入房间
    {
    }
    else if(messageMap["type"] == "bc_buy_deserve") //用户赠送酬勤通知消息
    {
        str = QString("%1 %2 %3 %4").arg(func(font_color_black,font_MS, QStringLiteral("酬勤赠送:")))
                .arg(func(font_color_red,font_MS, QStringLiteral("赠送数量:") +messageMap["cnt"]))
                .arg(func(font_color_blue,font_MS, QStringLiteral("酬勤等级:") +messageMap["lev"]))
                .arg(func(font_color_black,font_MS, QStringLiteral("用户信息:") +messageMap["sui"]));
    }
    else if(messageMap["type"] == "connectstate")
    {
        str = QString("%1 %2 %3").arg(func(font_color_red,font_Con,"["+messageMap["time"]+"]"))
                .arg(func(font_color_blue,font_MS, QStringLiteral("房间号:") +messageMap["roomid"]))
                .arg(func(font_color_black,font_MS,messageMap["state"]));
    }
    else
    {
        str = "";
    }
    return str;
}


QString StringGenerator::TE(QString color, QString font_family,QString txt)
{
    return QString("<font style=\"font-family:%1;color:%2\">%3</font>").arg(font_family)
            .arg(color).arg(txt);
}

QString StringGenerator::TT(QString color, QString font_family, QString txt)
{
    return txt;
}
