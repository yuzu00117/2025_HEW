/****************************************************************************
 *
 * Copyright (c) CRI Middleware Co., Ltd.
 *
 ****************************************************************************/
/*!
 *	\file		crisoundxr_le_plugin.h
 */

/* 多重定義防止 */
/* Prevention of redefinition */
#ifndef	CRI_INCL_SOUNDXR_LE_PLUGIN_H
#define	CRI_INCL_SOUNDXR_LE_PLUGIN_H

/***************************************************************************
 *      型定義
 *      Type Definition
 ***************************************************************************/

/***************************************************************************
 *      定数マクロ
 *      Macro Constants
 ***************************************************************************/

/*!
 * \brief モジュール名文字列
 * \par 説明:
 * CRI Sound xR モジュールを示す文字列です。
 */
/*
 * Module Name
 * Description:
 * A string indicating the CRI Sound xR module.
 */
#define CRISOUNDXR_MODULE_NAME	"CRI Sound xR"

/*!
 * \brief バージョン文字列
 * \par 説明:
 * CRI Sound xR モジュールのバージョンを示す文字列です。
 */
/*
 * Version String
 * Description:
 * A string indicating the version of the CRI Sound xR module.
 */
#define CRISOUNDXR_VERSION_STRING	"1.01.00"

/*!
 * \brief バージョン番号
 * \par 説明:
 * CRI Sound xR モジュールのバージョンを示す番号です。<br>
 * ::CRISOUNDXR_VERSION_STRING に対応します。
 */
/*
 * Version Number
 * Description:
 * A number indicating the version of the CRI Sound xR module.
 */
#define CRISOUNDXR_VERSION_NUM	(0x01010000)

#ifdef __cplusplus
extern "C" {
#endif

/*JP
 * \brief Sound xRの関数インタフェースの取得
 * \retrun Sound xRのバージョン情報付き関数インタフェース
 * \par 説明:
 * Sound xRのAPIをAtomライブラリ内で使用するためのインタフェースを取得します。<br>
 */
const void* criSoundxR_GetInterface(void);

#ifdef __cplusplus
}
#endif

#endif	/* CRI_INCL_SOUNDXR_LE_PLUGIN_H */
