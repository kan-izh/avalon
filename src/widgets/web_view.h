/*!
 * \file
 * \brief Cабклассинг QWebView, т.к. он криво масштабируется и установка дефолтного поведения
 */

#ifndef _avalon_web_view_h_
#define _avalon_web_view_h_

#include "sysheaders.h"
#include <functional>

/*!
 * \brief Cабклассинг QWebPage для установки User-Agent ресурсов, загружаемых из сообщений
 */
class AWebPage : public QWebEnginePage
{
    Q_OBJECT

    public:

        AWebPage(QObject* parent = NULL) : QWebEnginePage(parent) {}

	protected:

		QString userAgentForUrl (const QUrl& /*url*/) const { return getAgentString(); }

        bool acceptNavigationRequest(const QUrl &url, QWebEnginePage::NavigationType type, bool isMainFrame) override {
            if (type == NavigationTypeLinkClicked) {
                emit linkClicked(url);
                return false; // Prevent default browser navigation
            }
            return QWebEnginePage::acceptNavigationRequest(url, type, isMainFrame);
        }

    signals:
        void linkClicked(const QUrl &url);
};

/*!
 * \brief Cабклассинг QWebView для установки дефолтного поведения
 */
class AWebView : public QWebEngineView
{
	Q_OBJECT

	public:

		AWebView  (QWidget* parent);
		~AWebView () {}

		/*!
		 * \brief Флаг того, что мышь находится над ссылкой
		 * необходим для выдачи соответствующего QAction для возможности копирования ссылки, а не текста
		 * устанавливается в дочерних классах
		 */
		bool LinkHovered;

		/*!
		 * \brief Прокрутка страницы сообщения
		 */
        void scrollPage (std::function<void()> callback);

	protected:

		/*!
		 * \brief Кастомизация контекстного меню
		 */
		virtual void contextMenuEvent (QContextMenuEvent* event);

		/*!
		 * \brief Обработчик клавиш для реализации копирования по хоткею
		 */
		virtual void keyPressEvent (QKeyEvent* event);

	private slots:

		// меню
		void menu_yandex_triggered ();             /*!< \brief Поиск Яндекс         */
		void menu_wikipedia_triggered ();          /*!< \brief Поиск Википедии      */
		void menu_google_triggered ();             /*!< \brief Поиск Google         */
		void menu_google_translate_triggered ();   /*!< \brief Переводчик Google    */
		void menu_rsdn_triggered ();               /*!< \brief Поиск RSDN           */
};

/*!
 * \brief Cабклассинг AWebView, т.к. он криво масштабируется
 */
class AWebViewWidget : public QFrame
{
	Q_OBJECT

	public:

		AWebViewWidget  (QWidget* parent);
		~AWebViewWidget ();

		/*!
		 * \brief Объект сабклассинга
		 */
		AWebView* View;

	protected:

		/*!
		 * \brief Масштабирование
		 */
		void resizeEvent (QResizeEvent* event);
};

#endif   // _avalon_web_view_h_
