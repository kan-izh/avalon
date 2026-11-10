#include "message_view.h"
//----------------------------------------------------------------------------------------------
#include "global.h"
#include "formatter.h"
#include "storage/storage_factory.h"
//----------------------------------------------------------------------------------------------

AMessageView::AMessageView (QWidget* parent) : AWebViewWidget (parent), IMessageView ()
{
	m_main_form    = 0;
	m_forum_tree   = 0;
	m_message_tree = 0;
    AWebPage *page = qobject_cast<AWebPage*>(View->page());
    if(page) {
        connect(page, &AWebPage::linkClicked, this, &AMessageView::link_clicked);
    }
    connect(page, &AWebPage::linkHovered, this, &AMessageView::link_hover);
}
//----------------------------------------------------------------------------------------------

AMessageView::~AMessageView ()
{
}
//----------------------------------------------------------------------------------------------

void AMessageView::setMainForm (IFormMain* itf)
{
	m_main_form = itf;
}
//----------------------------------------------------------------------------------------------

void AMessageView::setForumTree (IForumTree* itf)
{
	m_forum_tree = itf;
}
//----------------------------------------------------------------------------------------------

void AMessageView::setMessageTree (IMessageTree* itf)
{
	m_message_tree = itf;
}
//----------------------------------------------------------------------------------------------

void AMessageView::setMessage (const AMessageInfo& message, const AForumInfo* forum)
{
	Q_ASSERT(forum != NULL && forum->IDGroup != SPECIAL_ID_GROUP);

	QString body;

	// загрузка рейтингов (если требуется)
	if (forum != NULL && forum->Rated == true)
	{
		// "тихий" контроль ошибок, т.к. не страшно, если пузомерка не загрузится
		QScopedPointer<IAStorage> storage(AStorageFactory::getStorage());

		if (storage.isNull() == false)
		{
			AMessageRatingList list;

			if (storage->getMessageRatingList(message.ID, list) != false)
				body = AFormatter::formatMessage(message, forum, true, (list.count() > 0 ? &list : NULL));
			else
				body = AFormatter::formatMessage(message, forum, true);
		}
		else
			body = AFormatter::formatMessage(message, forum, true);
	}
	else
		body = AFormatter::formatMessage(message, forum);

	View->setHtml(body);
}
//----------------------------------------------------------------------------------------------

void AMessageView::clear ()
{
	View->setHtml(QString());
}
//----------------------------------------------------------------------------------------------

void AMessageView::link_clicked (const QUrl& url)
{
    QString special = url.toString();

    if (m_message_tree != NULL)
        m_message_tree->processUrl(special);
    else
        QDesktopServices::openUrl(url);
}

void AMessageView::link_hover (const QString& link)
{
       bool avalon_link = link.indexOf("avalon:") == 0;

       if (link.length() != 0 && avalon_link == false)
               View->LinkHovered = true;
       else
               View->LinkHovered = false;

       if (avalon_link == false)
               m_main_form->showStatus(link);
}
//----------------------------------------------------------------------------------------------
