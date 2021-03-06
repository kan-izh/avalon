/*!
 * \file
 * \brief Заголовочный файл для глобальных настроек
 */

#ifndef _avalon_global_h_
#define _avalon_global_h_

#include "model/user.h"

/*!
 * \brief Глобальные настройки
 */
class AGlobal
{
	private:

		AGlobal ();

	public:

		/*!
		 * \brief Стандартный singleton
		 */
		static AGlobal* getInstance ();

		~AGlobal ();

		/*!
		 * \brief Перезагрузка настроек
		 */
		void reload ();

		//
		// Общие переменные
		//

		AUserInfo Me;            /*!< \brief Текущий пользователь avalon */
#ifdef AVALON_USE_ZLIB
		bool      Compression;   /*!< \brief Сжатие тел сообщений        */
#endif

		//
		// Константы форматирования
		//

		QString AnonymousName;   /*!< \brief Отображаемое имя анонима */
		QString DateFormat;      /*!< \brief Формат даты/времени      */

		/*!
		 * \brief Протокол работы с сайтом rsdn (http|https)
		 * \return Имя протокола http или https
		 */
		const QString& rsdnProto() const { return m_rsdn_proto; }

		/*!
		 * \brief Базовое имя хоста работы с сайтом rsdn
		 * \return Имя хоста (rsdn.org, rsdn.ru, etc)
		 */
		const QString& rsdnHost() const { return m_rsdn_host; }

		/*!
		 * \brief Базовый URL работы с сайтом rsdn
		 * \return URL вида https://rsdn.org
		 */
		const QString& rsdnUrl() const { return m_rsdn_url; }

	private:

		//
		// Сайт и вебсервис
		//

		QString m_rsdn_proto;   /*!< \brief Протокол (http|https) */
		QString m_rsdn_host;    /*!< \brief Имя хоста для rsdn    */
		QString m_rsdn_url;     /*!< \brief URL для rsdn          */
};

#endif   // _avalon_global_h_
