#include "formatter.h"
//----------------------------------------------------------------------------------------------
#include "global.h"
//----------------------------------------------------------------------------------------------

QString AFormatter::headHTML ()
{
#ifndef AVALON_PACKAGE
	QString path = QCoreApplication::applicationDirPath();
#else
	QString path = QDir::homePath() + "/.avalon";
#endif

	QString custom_css;
	if (QFileInfo(path + "/avalon.css").exists() == true)
#ifdef Q_WS_WIN
		custom_css = "	<link rel='stylesheet' href='file:///" + path + "/avalon.css' type='text/css' media='screen' />\n";
#else
		custom_css = "	<link rel='stylesheet' href='file://" + path + "/avalon.css' type='text/css' media='screen' />\n";
#endif

	// https://tech.yandex.ru/jslibs/#highlight
	QStringList lang;
	lang << "avrasm" << "bash" << "cmake" << "cpp" << "cs" << "css" << "delphi" << "diff" << "dos" << "erlang"
		 << "haskell" << "http" << "ini" << "java" << "javascript" << "json" << "lisp" << "lua" << "perl"
		 << "php" << "python" << "ruby" << "rust" << "sql" << "vbscript" << "xml";

	QString langs;
	for (int i = 0; i < lang.count(); i++)
		langs += "	<script src='https://yastatic.net/highlightjs/8.2/languages/" + lang[i] + ".min.js'></script>\n";

	QString result =
		"<!DOCTYPE html>\n"
		"<html xmlns='http://www.w3.org/1999/xhtml'>\n"
		"<head>\n"
		"	<meta charset='utf-8' />\n"
		"	<link rel='stylesheet' href='qrc:/style.css' type='text/css' media='screen' />\n"
		+ custom_css +
		"	<link rel='stylesheet' title='Magula' href='https://yastatic.net/highlightjs/8.2/styles/magula.min.css' />\n"
		"	<script src='https://yastatic.net/highlightjs/8.2/highlight.min.js'></script>\n"
		+ langs +
		"	<script>hljs.tabReplace = '    '; hljs.initHighlightingOnLoad();</script>\n"
		"</head>\n"
		"<body>\n";

	return result;
}
//----------------------------------------------------------------------------------------------

QString AFormatter::subjectHTML (const AMessageInfo& message, const AForumInfo* forum)
{
	// тема письма хранится без html замен
	QString subject = message.Subject;

	subject.replace("&", "&amp;");
	subject.replace("<", "&lt;");
	subject.replace(">", "&gt;");

	QString pad    = "			";
	QString result = pad + "<div id='title'>\n";

	if (forum != NULL)
	{
		result +=
			pad + "	<a id='move_to_thread' href='" + AGlobal::getInstance()->rsdnUrl() + "/forum/" + forum->ShortName + "/" + QString::number(message.ID) + QString::fromUtf8("'><img src='qrc:/icons/show_topic.png' title='показать положение в теме' /></a>\n") +
			pad + "	<a id='subject' href='" + AGlobal::getInstance()->rsdnUrl() + "/forum/" + forum->ShortName + "/" + QString::number(message.ID) + ".1'>" + subject + "</a>\n";
	}
	else
		result += pad + "<p id='subject'>" + subject + "</p>\n";

	result += pad + "</div>\n";

	return result;
}
//----------------------------------------------------------------------------------------------

QString AFormatter::rateHTML (bool rated, bool moderated)
{
	QString pad = "				";
	QString result = pad + "<div id='rate'>\n";

	if (rated == true)
	{
		result +=
			pad + QString::fromUtf8("<a id='rate_plus_1' href='avalon:rate_plus_1'><img src='qrc:/icons/rate_plus_1.png' title='+1' /></a>\n") +
			pad + QString::fromUtf8("<a id='rate_1' href='avalon:rate_1'><img src='qrc:/icons/rate_1.png' title='интересно' /></a>\n") +
			pad + QString::fromUtf8("<a id='rate_2' href='avalon:rate_2'><img src='qrc:/icons/rate_2.png' title='спасибо' /></a>\n") +
			pad + QString::fromUtf8("<a id='rate_3' href='avalon:rate_3'><img src='qrc:/icons/rate_3.png' title='супер' /></a>\n") +
			pad + QString::fromUtf8("<a id='rate_cross' href='avalon:rate_cross'><img src='qrc:/icons/rate_cross.png' title='удалить оценку' /></a>\n") +
			pad + QString::fromUtf8("<a id='rate_smile' href='avalon:rate_smile'><img src='qrc:/icons/rate_smile.png' title='смешно' /></a>\n") +
			pad + QString::fromUtf8("<a id='rate_plus' href='avalon:rate_plus'><img src='qrc:/icons/rate_plus.png' title='согласен' /></a>\n") +
			pad + QString::fromUtf8("<a id='rate_minus' href='avalon:rate_minus'><img src='qrc:/icons/rate_minus.png' title='не согласен' /></a>\n");
	}

	if (moderated == true)
		result += pad + QString::fromUtf8("<a id='moderate' href='avalon:moderate'><img src='qrc:/icons/moderate.png' title='модерирование' /></a>\n");

	result += pad + "</div>\n";

	return result;
}
//----------------------------------------------------------------------------------------------

QString AFormatter::authorHTML (int id, const QString nick)
{
	QString pad = "			";
	QString result;

	if (id == 0 || id == -1)
		result += pad + QString::fromUtf8("<div class='info_left'>От:</div><div class='info_right'>") + nick + "</div><br />\n";
	else
		result += pad + QString::fromUtf8("<div class='info_left'>От:</div><div class='info_right'><a href='") + AGlobal::getInstance()->rsdnUrl() + "/account/info/" + QString::number(id) + "'>" + nick + "</a></div><br />\n";

	return result;
}
//----------------------------------------------------------------------------------------------

QString AFormatter::ratingHTML (int id, const AMessageRatingList* rating_list)
{
	if (rating_list == NULL || rating_list->count() == 0)
		return "";

	QString result = "";

	int rate_funny = 0;
	int rate_thanks = 0;
	int rate_thanks_count = 0;
	int rate_agree = 0;
	int rate_disagree = 0;

	QString rate_details[7];
	for (int i = 0; i < rating_list->count(); i++)
	{
		AMessageRating info = rating_list->at(i);
		int detailIdx=-1;
		switch(info.Rate)
		{
		case 1:
			rate_thanks += info.UserRating;
			rate_thanks_count++;
			detailIdx=0;
			break;
		case 2:
			rate_thanks += 2 * info.UserRating;
			rate_thanks_count++;
			detailIdx=1;
			break;
		case 3:
			rate_thanks += 3 * info.UserRating;
			rate_thanks_count++;
			detailIdx=2;
			break;
		case 0:
			rate_disagree++;
			detailIdx=3;
			break;
		case -2:
			rate_funny++;
			detailIdx=4;
			break;
		case -3:
			rate_thanks++;
			rate_thanks_count++;
			detailIdx=5;
			break;
		case -4:
			rate_agree++;
			detailIdx=6;
			break;
		}
		static const QString rateText[] = {"[1]", "[2]", "[3]", "[-]", ":)", "[+1]", "[+]"};
		if(detailIdx >= 0)
			rate_details[detailIdx] += rateText[detailIdx] + (info.Nick.isEmpty() ? info.Name : info.Nick) + " ";
	}
	QString all_rate_details =
			rate_details[5]
			+ rate_details[0]
			+ rate_details[1]
			+ rate_details[2]
			+ rate_details[3]
			+ rate_details[6]
			+ rate_details[4]
			;

	QString pad = "			";
	QString ratingLinkOpen = "<a id='rating' title='"+all_rate_details+"' href='" + AGlobal::getInstance()->rsdnUrl() + "/forum/RateList.aspx?mid=" + QString::number(id) + "'>";
	result +=
		pad + ratingLinkOpen + "<span class='info_left'>" + QString::fromUtf8("Оценки:</span>\n") +
		pad + "<span class='info_right'>\n";

	if (rate_thanks > 0)
		result += pad + QString::number(rate_thanks) + " (" + QString::number(rate_thanks_count) + ") ";
	if (rate_agree > 0)
		result += pad + QString::fromUtf8("+") + QString::number(rate_agree) + " ";
	if (rate_disagree > 0)
		result += pad + QString::fromUtf8("-") + QString::number(rate_disagree) + " ";
	for(int i=0; i<rate_funny/3; ++i)
		result += pad + QString::fromUtf8("<img id='rating_biggrin' src='qrc:/smiles/biggrin.png' align='top' alt=':)))'>");
	if(rate_funny % 3 == 2)
		result += pad + QString::fromUtf8("<img id='rating_grin' src='qrc:/smiles/grin.png' align='top' alt=':))'>");
	if(rate_funny % 3 == 1)
		result += pad + QString::fromUtf8("<img id='rating_smile' src='qrc:/smiles/smile.png' align='top'' alt=':)'>");

	result += pad + "</span></a><br />\n";

	return result;
}
//----------------------------------------------------------------------------------------------

QString AFormatter::footerHTML ()
{
	QString result =
		"	<div id='footer'></div>\n"
		"</body>\n"
		"</html>\n";

	return result;
}
//----------------------------------------------------------------------------------------------

QString AFormatter::formatMessage (const AMessageInfo& message, const AForumInfo* forum, bool rated, const AMessageRatingList* rating_list)
{
	Q_ASSERT(forum != NULL && forum->IDGroup != SPECIAL_ID_GROUP);

	// парсинг сообщения
	AParsedBlockList list = AParser::parseBlocks(message.Message);

	// шапка html
	QString result = headHTML() +
	"	<div id='message'>\n"
	"		<div id='header'>\n";

	// заголовок
	result += subjectHTML(message, forum);

	// иконки для выставления оценок
	result += rateHTML(rated, forum != NULL);

	// закрытие div.id = header
	result +=
	"		</div>\n"
	"		<div id='info'>\n";

	// автор сообщения + ссылка на профиль
	result += authorHTML(message.IDUser, message.UserNick.length() > 0 ? message.UserNick : QString::fromUtf8("Аноним"));

	// дата сообщения
	result += QString::fromUtf8("			<div class='info_left'>Дата:</div><div class='info_right'>") + message.MessageDate.toString("dd.MM.yyyy HH:mm") + "</div><br />\n";

	// рейтинг сообщения
	result += ratingHTML(message.ID, rating_list);

	// закрытие div.id = info
	result += "		</div>\n";

	// формирование тела сообщения
	result += "		<div id='text'>\n";

	for (int i = 0; i < list.count(); i++)
		result += formatParsedBlock(list.at(i));

	result += "		</div>\n";

	// закрытие div.id = message
	result += "	</div>\n";

	// хвост html
	result += footerHTML();
	return result;
}
//----------------------------------------------------------------------------------------------

QString AFormatter::formatParsedBlock (const AParsedBlock& block)
{
	QString pad = "			";
	QString result;

	if (block.Type == pbtText)
	{
		result += formatQuotedStringList(block.Strings, block.Type, block.SubType);
	}

	else if (block.Type == pbtQuote)
	{
		result += pad + "<blockquote>\n";
		result += formatQuotedStringList(block.Strings, block.Type, block.SubType);
		result += pad + "</blockquote>\n";
	}

	else if (block.Type == pbtModerator)
	{
		result += pad + "<div class='moderator'>\n";
		result += formatQuotedStringList(block.Strings, block.Type, block.SubType);
		result += pad + "</div>\n";
	}

	else if (block.Type == pbtTagline)
	{
		result += pad + "<div class='tagline'>\n";
		result += formatQuotedStringList(block.Strings, block.Type, block.SubType);
		result += pad + "</div>\n";
	}

	else if (block.Type == pbtTable)
	{
		result += pad + "<table><tbody>\n";
		result += formatQuotedStringList(block.Strings, block.Type, block.SubType);
		result += pad + "</tbody></table>\n";
	}

	else if (block.Type == pbtCut)
	{
		result += "<details class='cut-block'><summary>";
		result += block.Title.isEmpty() ? "cut" : block.Title;
		result += "</summary><div class='cut-content'>";
		for (int i = 0; i < block.SubBlocks.count(); i++)
			result += formatParsedBlock(block.SubBlocks.at(i));
		result += "</div></details>";
	}

	else
	{
		/*!
		 * \brief Описатель соответствий языку и имени для подсветки
		 */
		typedef struct AHighlightMap
		{
			AParsedBlockType Type;      /*!< \brief Тип кода для подсветки    */
			const char*      CodeClass; /*!< \brief Класс кода в Highlight.js */
		} AHighlightMap;

		// Список замены для подсветки синтаксиса Highlight.js
		// см. http://softwaremaniacs.org/soft/highlight/
		const AHighlightMap highlight_map [] =
		{
			{ pbtCode,      ""         },
			{ pbtAssembler, "avrasm"   },
			{ pbtC,         "cpp"      },
			{ pbtCPP,       "cpp"      },
			{ pbtCSharp,    "cs"       },
			{ pbtMSIL,      "cpp"      },
			{ pbtIDL,       "cpp"      },
			{ pbtPascal,    "delphi"   },
			{ pbtBasic,     "vbscript" },
			{ pbtSQL,       "sql"      },
			{ pbtPerl,      "perl"     },
			{ pbtPHP,       "php"      },
			{ pbtJava,      "java"     },
			{ pbtXML,       "xml"      },
			{ pbtLisp,      "lisp"     },
			{ pbtHaskell,   "haskell"  },
			{ pbtErlang,    "erlang"   },
			{ pbtOCaml,     "lisp"     },
			{ pbtProlog,    "lisp"     },
			{ pbtPython,    "python"   },
			{ pbtPython,    "rust"     },
			{ pbtRuby,      "ruby"     },
			{ pbtNemerle,   "cs"       },
			{ pbtText,      NULL       }  // last
		};

		// массив тэгов для подсветки кода
		const AHighlightMap* map = highlight_map;

		QString code_class;

		while (map->CodeClass != NULL)
		{
			if (block.Type == map->Type)
			{
				code_class = QString::fromUtf8(map->CodeClass);
				break;
			}

			map++;
		}

		if (code_class.length() > 0)
			result += pad + "<pre><code class='" + code_class + "'>";
		else
			result += pad + "<pre><code>";

		result += formatQuotedStringList(block.Strings, block.Type, block.SubType);

		result += "</code></pre>\n";
	}

	return result;
}
//----------------------------------------------------------------------------------------------

QString AFormatter::formatQuotedStringList (const AQuotedStringList& list, AParsedBlockType type, AParsedBlockSubType sub_type)
{
	QString result;

	int count = list.count();

	for (int i = 0; i < count; i++)
	{
		AQuotedString string = list.at(i);

		QString line = string.Data;

		if (string.QuoteText.length() > 0)
			line = string.QuoteText + " " + line;

		if (type != pbtTable && sub_type != pbstSourceCode)
		{
			QString pad = "			";

			if (string.QuoteLevel != 0)
			{
				if (string.QuoteLevel % 3 == 0)
					result += pad + "<p class='quote_blue'> " + line + " </p>\n";
				else if (string.QuoteLevel % 2 == 0)
					result += pad + "<p class='quote_red'> " + line + " </p>\n";
				else
					result += pad + "<p class='quote_green'> " + line + " </p>\n";
			}
			else
				result += pad + "<p class='quote_black'> " + line + " </p>\n";
		}
		else
		{
			if (i != count - 1)
				result += line + "\n";
			else
				result += line;
		}
	}

	if (sub_type != pbstSourceCode)
		result = formatSimpleText(result);

	return result;
}
//----------------------------------------------------------------------------------------------

QString AFormatter::formatSimpleText (const QString& text)
{
	/*!
	 * \brief Описатель соответствий простых BB тэгов и их html замен
	 */
	typedef struct ASimpleTag
	{
		const char* Source;  /*!< \brief Имя тэга (со скобками) */
		const char* Replace; /*!< \brief Текст замены           */
	} ASimpleTag;

	// Список замены BB кодов
	const ASimpleTag replace_map [] =
	{
		{ "[tt]",       "<tt>"     },
		{ "[/tt]",      "</tt>"    },
		{ "[b]",       "<b>"       },
		{ "[/b]",      "</b>"      },
		{ "[i]",       "<i>"       },
		{ "[/i]",      "</i>"      },
		{ "[u]",       "<u>"       },
		{ "[/u]",      "</u>"      },
		{ "[s]",       "<strike>"  },
		{ "[/s]",      "</strike>" },
		{ "[tr]",      "<tr>"      },
		{ "[/tr]",     "</tr>"     },
		{ "[td]",      "<td>"      },
		{ "[/td]",     "</td>"     },
		{ "[th]",      "<th>"      },
		{ "[/th]",     "</th>"     },
		{ "[hr]",      "<hr />"    },
		{ "[h1]",      "<h1>"      },
		{ "[/h1]",     "</h1>"     },
		{ "[h2]",      "<h2>"      },
		{ "[/h2]",     "</h2>"     },
		{ "[h3]",      "<h3>"      },
		{ "[/h3]",     "</h3>"     },
		{ "[h4]",      "<h4>"      },
		{ "[/h4]",     "</h4>"     },
		{ "[h5]",      "<h5>"      },
		{ "[/h5]",     "</h5>"     },
		{ "[h6]",      "<h6>"      },
		{ "[/h6]",     "</h6>"     },
		{ "[*]",       "<li>"      },
		{ ":)))",      "<img src='qrc:/smiles/sideways.png'>"   },
		{ ":))",       "<img src='qrc:/smiles/biggrin.png'>"    },
		{ ":)",        "<img src='qrc:/smiles/smile.png'>"      },
		{ ":(",        "<img src='qrc:/smiles/sad.png'>"        },
		{ ";)",        "<img src='qrc:/smiles/wink.png'>"       },
		{ ":-\\",      "<img src='qrc:/smiles/pouty.png'>"      },
		{ ":???:",     "<img src='qrc:/smiles/wacko.png'>"      },
		{ ":no:",      "<img src='qrc:/smiles/stop.png'>"       },
		{ ":up:",      "<img src='qrc:/smiles/thumbs_up.png'>"  },
		{ ":down:",    "<img src='qrc:/smiles/thumb_down.png'>" },
		{ ":super:",   "<img src='qrc:/smiles/cool.png'>"       },
		{ ":shuffle:", "<img src='qrc:/smiles/blush.png'>"      },
		{ ":wow:",     "<img src='qrc:/smiles/shock.png'>"      },
		{ ":crash:",   "<img src='qrc:/smiles/angry.png'>"      },
		{ ":user:",    "<img src='qrc:/smiles/music.png'>"      },
		{ ":maniac:",  "<img src='qrc:/smiles/devil.png'>"      },
		{ ":xz:",      "<img src='qrc:/smiles/wrong.png'>"      },
		{ ":beer:",    "<img src='qrc:/smiles/beer.png'>"       }
	};

	QString result = text;

	for (size_t i = 0; i < sizeof(replace_map) / sizeof(ASimpleTag); i++)
        result.replace(QString::fromUtf8(replace_map[i].Source), QString::fromUtf8(replace_map[i].Replace), Qt::CaseInsensitive);

	//
	// [img] ... [/img]
	//

    QRegularExpression img("\\[img(=\\w+)?\\](.+?)\\[/img\\]", QRegularExpression::CaseInsensitiveOption);
    QRegularExpressionMatchIterator it = img.globalMatch(result);

    if (it.hasNext()) {
        QString optimizedResult;
        optimizedResult.reserve(result.size());

        int lastIndex = 0;
        while (it.hasNext()) {
            QRegularExpressionMatch match = it.next();

            // 1. Append the chunk of text between the last match and this match
            optimizedResult.append(result.mid(lastIndex, match.capturedStart() - lastIndex));

            // 2. Process the tag
            QString lstr = match.captured(2).trimmed();
            int lval = AParser::isURL(lstr);
            QString html;

            if (lval == 1)
                optimizedResult.append("<center><img src='").append(lstr).append("'></center>");
            else if (lval == 2) // опасная ссылка
                optimizedResult.append("<span class='alert'>").append(lstr).append("</span>");
            else // невалидная ссылка
                optimizedResult.append(lstr);

            lastIndex = match.capturedEnd();
        }

        // 4. Append any remaining text after the final match
        optimizedResult.append(result.mid(lastIndex));
        result = optimizedResult;
    }

	//
	// списки
	//

    QRegularExpression list1("\\[list=1\\](.+?)\\[/list\\]", QRegularExpression::CaseInsensitiveOption);
    QRegularExpression list2("\\[list=a\\](.+?)\\[/list\\]", QRegularExpression::CaseInsensitiveOption);
    QRegularExpression list3("\\[list\\](.+?)\\[/list\\]",   QRegularExpression::CaseInsensitiveOption);

	result.replace(list1, "<ol type='1' start='1'>\\1</ol>");
	result.replace(list2, "<ol type='a'>\\1</ol>");
	result.replace(list3, "<ul>\\1</ul>");

	// гиперссылки
	result = formatHyperlinks(result);

	result = "			" + result;

	return result;
}
//----------------------------------------------------------------------------------------------
QString AFormatter::formatHyperlinks(const QString& text)
{
    // Helper lambda to perform an O(N) single-pass regex replacement
    auto processRegexPass = [](const QString &input, const QRegularExpression &regex, auto replacer) -> QString {
        QRegularExpressionMatchIterator it = regex.globalMatch(input);
        if (!it.hasNext()) {
            return input;
        }

        QString output;
        output.reserve(input.size() + 256);

        int lastIndex = 0;
        while (it.hasNext()) {
            QRegularExpressionMatch match = it.next();

            // 1. Append text between the last match and the current match
            output.append(input.mid(lastIndex, match.capturedStart() - lastIndex));

            // 2. Generate replacement via callback
            output.append(replacer(match));

            lastIndex = match.capturedEnd();
        }

        // 3. Append remaining text after the final match
        output.append(input.mid(lastIndex));
        return output;
    };

    QString result = text;

    // 1. [url]...[/url]
    QRegularExpression url1("\\[url\\](.+?)\\[/url\\]", QRegularExpression::CaseInsensitiveOption);
    result = processRegexPass(result, url1, [](const QRegularExpressionMatch &match) {
        QString lstr = match.captured(1);
        int lval = AParser::isURL(lstr);

        if (lval == 1)
            return QString("<a href='%1'>%1</a>").arg(lstr);
        else if (lval == 2) // опасная ссылка
            return QString("<span class='alert'>%1</span>").arg(lstr);
        else // невалидная ссылка
            return lstr;
    });

    // 2. [url=...]...[/url]
    QRegularExpression url2("\\[url=(\\S+?)\\](.+?)\\[/url\\]", QRegularExpression::CaseInsensitiveOption);
    result = processRegexPass(result, url2, [](const QRegularExpressionMatch &match) {
        QString lstr = match.captured(1);
        QString rstr = match.captured(2);

        int lval = AParser::isURL(lstr);
        int rval = AParser::isURL(rstr);

        if (lval == 1)
            return QString("<a href='%1'>%2</a>").arg(lstr, rstr);
        else if (rval == 1)
            return QString("<a href='%2'>%1</a>").arg(lstr, rstr);
        else if (lval == 2)
            return QString("<span class='alert'>%2</span>").arg(lstr, rstr);
        else if (rval == 2)
            return QString("<span class='alert'>%1</span>").arg(lstr, rstr);
        else // невалидная ссылка
            return QString("%1 (%2)").arg(rstr, lstr);
    });

    // 3. Raw URLs without tags
    QRegularExpression url3("[^'>]((http://|https://|ftp://|[^/]www\\.)[^<\\s]+)", QRegularExpression::CaseInsensitiveOption);
    result = processRegexPass(result, url3, [](const QRegularExpressionMatch &match) {
        // match.captured(0) includes the leading non-'>' character,
        // match.captured(1) is the actual URL group.
        QString fullMatch = match.captured(0);
        QString lstr = match.captured(1);
        int lval = AParser::isURL(lstr);

        char prefix = fullMatch.at(0).toLatin1();

        if (lval == 1) {
            QString decoded = QUrl::fromPercentEncoding(lstr.toUtf8());
            QString html = QString("<a href='%1'>%2</a>").arg(lstr, decoded);
            // Preserve the leading character (e.g., space or quote boundary)
            return QString(prefix) + html; // or properly align offsets
        }
        return fullMatch;
    });

    // 4. [email]...[/email]
    QRegularExpression email("\\[email\\](\\S+?)\\[/email\\]", QRegularExpression::CaseInsensitiveOption);
    result = processRegexPass(result, email, [](const QRegularExpressionMatch &match) {
        QString lstr = match.captured(1);
        int lval = AParser::isURL(lstr);

        if (lval == 1)
            return QString("<a href='mailto:%1'>%1</a>").arg(lstr);
        return lstr;
    });

    // 5. [msdn]...[/msdn]
    // Native Qt 6 global replace works cleanly here since substitution patterns (\1) are fully supported.
    QRegularExpression msdn("\\[msdn\\](\\S+?)\\[/msdn\\]", QRegularExpression::CaseInsensitiveOption);
    result.replace(msdn, "<a href='http://search.msdn.microsoft.com/Default.aspx?brand=Msdn&query=\\1'>\\1</a>");

    return result;
}

//----------------------------------------------------------------------------------------------

QString AFormatter::normalizeBody (const QString& body, const QString& nick)
{
    QString data = body;

    // отбрасываем "[tagline]" и "[moderator]"
    QRegularExpression tagline("\\[tagline\\](.+?)\\[/tagline\\]", QRegularExpression::CaseInsensitiveOption);
    QRegularExpression moderator("\\[moderator\\](.+?)\\[/moderator\\]", QRegularExpression::CaseInsensitiveOption);

    data.replace(tagline, "");
    data.replace(moderator, "");

    // удаляем img с данными вместо ссылки (например, см. https://rsdn.org/forum/flame.comp/4077971.1)
    QRegularExpression img1("\\[img\\]data:(\\S+?)\\[/img\\]", QRegularExpression::CaseInsensitiveOption);
    data.replace(img1, "");

    // заменяем [img]...[/img] на ссылку
    QRegularExpression img2("\\[img\\](\\S+?)\\[/img\\]", QRegularExpression::CaseInsensitiveOption);
    data.replace(img2, "[url=\\1]\\1[/url]");

    // укорачивание длинных ссылок (например, см. https://rsdn.org/forum/web/4086359.1)
    QRegularExpression url1("\\[url=data:(\\S+?)\\](.+?)\\[/url\\]", QRegularExpression::CaseInsensitiveOption);
    data.replace(url1, "[url=bad%20link]\\2[/url]");

    // удаление тэга цитирования
    QRegularExpression q1("(^|[^\\[])\\[q\\]", QRegularExpression::CaseInsensitiveOption | QRegularExpression::MultilineOption);
    data.replace(q1, "\\1");

    QRegularExpression q2("(^|[^\\[])\\[/q\\]", QRegularExpression::CaseInsensitiveOption | QRegularExpression::MultilineOption);
    data.replace(q2, "\\1");

    // удаление таблиц из цитирования
    QRegularExpression table("(^|[^\\[])\\[t\\](.+?)\\[/t\\]", QRegularExpression::CaseInsensitiveOption | QRegularExpression::MultilineOption);
    data.replace(table, "\\1");

    // удаление тэгов [h1]..[h6] из цитирования
    for (int i = 1; i < 7; i++)
    {
        QRegularExpression h1("(^|[^\\[])\\[h" + QString::number(i) + "\\]", QRegularExpression::CaseInsensitiveOption | QRegularExpression::MultilineOption);
        data.replace(h1, "\\1");

        QRegularExpression h2("(^|[^\\[])\\[/h" + QString::number(i) + "\\]", QRegularExpression::CaseInsensitiveOption | QRegularExpression::MultilineOption);
        data.replace(h2, "\\1");
    }

    data = data.trimmed();

    // разбиение
	QStringList source = data.split("\n");

	QString nick_3 = AFormatter::nick3(nick);

	// добавление квотинга к строкам
	for (int i = 0; i < source.size(); i++)
		if (source[i].trimmed().length())
			source[i] = nick_3 + "> " + source[i];

	// удаление дублирующихся пустых строк
	int index = 0;

	// регексп для приветствий в квотинге
    QRegularExpression hello(QString::fromUtf8(">\\s{0,}Здравствуйте,\\s.+,\\sВы писали:"));

	while (index < source.size() - 1)
	{
		if (source.at(index).trimmed().length() == 0 && source.at(index + 1).trimmed().length() == 0)
		{
			source.removeAt(index);
			continue;
		}
		else if (source.at(index).indexOf(hello) != -1)
		{
			source.removeAt(index);

			if (index > 0)
				index--;

			continue;
		}

		index++;
	}

	// вычисление уровня квотинга
	QList<int> quoting_level;

	int size = source.size();

	for (int i = 0; i < size; i++)
	{
		QString temp = source.at(i).trimmed();

		int level = 0;

		for (int j = 0; j < temp.length() - 3; j++)
		{
			if (temp[j] == ' ' && !(temp[j + 1] == '&' || temp[j + 2] == '&' || temp[j + 3] == '&'))
				break;
			else if (temp[j] == '&' && temp[j + 1] == 'g' && temp[j + 2] == 't' && temp[j + 3] == ';')
			{
				level++;
				j += 3;
			}
			else if (temp[j] == '&' && temp[j + 1] == 'l' && temp[j + 2] == 't' && temp[j + 3] == ';')
				break;
		}

		quoting_level.append(level);

		source[i] = temp;
	}

	// вставка пустых строк между разными уровнями квотинга
	for (int i = 0; i < size - 1; i++)
	{
		if (source[i].length() == 0)
			continue;

		if (quoting_level.at(i) != quoting_level.at(i + 1) && source[i + 1].length() /*&& (quoting_level.at(i) == 0 || quoting_level.at(i + 1) == 0)*/)
			source[i] = source[i] + "\r\n";
	}

	data = source.join("\r\n").trimmed();

	// замена HTML спец-символов
	data.replace("&gt;",  ">");
	data.replace("&lt;",  "<");
	data.replace("&amp;", "&");

	return data;
}
//----------------------------------------------------------------------------------------------

QString AFormatter::nick3 (const QString& nick)
{
	if (nick.length() < 4)
		return nick;

	QString result;

	for (int i = 0; i < nick.length(); i++)
		if (nick[i].isUpper() == true)
			result += nick[i];

	if (result.length() == 0)
		result = nick[0];

	if (result.length() > 3)
		result.resize(3);

	return result;
}
//----------------------------------------------------------------------------------------------
