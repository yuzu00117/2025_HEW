/****************************************************************************
 *
 * Copyright (c) CRI Middleware Co., Ltd.
 *
 ****************************************************************************/
/*!
 *	\file		crisoundxr_le_plugin.h
 */

/* ���d��`�h�~ */
/* Prevention of redefinition */
#ifndef	CRI_INCL_SOUNDXR_LE_PLUGIN_H
#define	CRI_INCL_SOUNDXR_LE_PLUGIN_H

/***************************************************************************
 *      �^��`
 *      Type Definition
 ***************************************************************************/

/***************************************************************************
 *      �萔�}�N��
 *      Macro Constants
 ***************************************************************************/

/*!
 * \brief ���W���[����������
 * \par ����:
 * CRI Sound xR ���W���[��������������ł��B
 */
/*
 * Module Name
 * Description:
 * A string indicating the CRI Sound xR module.
 */
#define CRISOUNDXR_MODULE_NAME	"CRI Sound xR"

/*!
 * \brief �o�[�W����������
 * \par ����:
 * CRI Sound xR ���W���[���̃o�[�W����������������ł��B
 */
/*
 * Version String
 * Description:
 * A string indicating the version of the CRI Sound xR module.
 */
#define CRISOUNDXR_VERSION_STRING	"1.01.00"

/*!
 * \brief �o�[�W�����ԍ�
 * \par ����:
 * CRI Sound xR ���W���[���̃o�[�W�����������ԍ��ł��B<br>
 * ::CRISOUNDXR_VERSION_STRING �ɑΉ����܂��B
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
 * \brief Sound xR�̊֐��C���^�t�F�[�X�̎擾
 * \retrun Sound xR�̃o�[�W�������t���֐��C���^�t�F�[�X
 * \par ����:
 * Sound xR��API��Atom���C�u�������Ŏg�p���邽�߂̃C���^�t�F�[�X���擾���܂��B<br>
 */
const void* criSoundxR_GetInterface(void);

#ifdef __cplusplus
}
#endif

#endif	/* CRI_INCL_SOUNDXR_LE_PLUGIN_H */
