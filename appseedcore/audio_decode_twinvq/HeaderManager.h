/* (c)Copyright 1996-2000 NTT Cyber Space Laboratories */
/*                Modified on 2000.09.06 by N. Iwakami */

// HeaderManager.h: CHeaderManager �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HEADERMANAGER_H__E059C7C4_6C3B_11D4_A71A_00C04F012175__INCLUDED_)
#define AFX_HEADERMANAGER_H__E059C7C4_6C3B_11D4_A71A_00C04F012175__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <map>
#include "Chunk.h"


class CHeaderManager  
{
public:
	typedef std::map<std::string, CChunk> CChunkBank;	// �`�����N�o���N�^�AID���L�[�Ƀ`�����N���i�[���Ă���}�b�v
	typedef std::string string;							// ������^

private:
	// �����o�ϐ��i�N���X�j
	CChunkBank m_primaryChunkBank;			// �ʏ�`�����N�̃`�����N�o���N
	CChunkBank m_secondaryChunkBank;		// �⏕�`�����N�̃`�����N�o���N
	string     m_chunkID;					// TWIN �`�����N��ID�A�ʏ��ID�ƈႢ "TWIN"+<�o�[�W�������ʎq>�ō\�������B

	// �����o�֐�
	CChunk GetChunk( CChunkBank *theBank, string id );						// �`�����N�o���N����ID�Ń`�����N�������o��
	void PickUpSubChunks ( CChunkBank* theChunkBank, CChunkChunk *inputChunk );	// Chunk�^�̃`�����N����T�u�`�����N�����o���`�����N�o���N�ɓo�^����
	void Init( CChunkChunk& twinChunk );	// ����������B�R���X�g���N�^�̑���Ɏg��
	CHeaderManager() { };					// �R���X�g���N�^�B���[�U�͌ĂׂȂ��B����� Create() ���g���B
											// �������̍ۂɃG���[���o��\�������邽�߂��̂悤�Ȏd�l�ɂ����B

	// �N���X�̒������Ŏg�����O
	class err_WrongChunkFormat { };		// �`�����N�̏������������Ȃ�

public:
	CChunk GetPrimaryChunk( string id )   { return GetChunk( &m_primaryChunkBank,   id ); }; // �ʏ�`�����N�������o��
	CChunk GetSecondaryChunk( string id ) { return GetChunk( &m_secondaryChunkBank, id ); }; // �⏕�`�����N�������o��
	const string  GetID() { return m_chunkID; };

	static CHeaderManager* Create( CChunkChunk& twinChunk );	// �`�����N�}�l�[�W�������o���B�R���X�g���N�^�̑���
	virtual ~CHeaderManager();	// �f�X�g���N�^

	// ��O
	class err_FailGetChunk { };			// �`�����N�̎擾�Ɏ��s����

};

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// �w�b�_�}�l�[�W������̃f�[�^�ǂݏo���̎x���N���X
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

// CUniStringInfo �N���X�̃C���^�[�t�F�C�X
// Unified string information, ������`�����N�̑������A�w�b�_�}�l�[�W������擾���邱�Ƃ��ł���
//
//////////////////////////////////////////////////////////////////////////////////////////////////
class CUniStringInfo
{
public:
	typedef std::string string;
	typedef std::string string;
	enum CharCode {
		unknown_code = -1,
		ISO_8859_1,
		Unicode,
		S_JIS,
		JIS,
		EUC,
	};
	
private:
	string m_id;				// �`�����N ID
	string m_primary;			// ��{������
	string m_secondary;			// �⏕������
	int m_primaryCharCode;		// ��{������̕����R�[�h
	int m_secondaryCharCode;	// �⏕������̕����R�[�h

	void PutPrimaryInfo( CStringChunk& theChunk );
	void PutSecondaryInfo( CStringChunk& theChunk );

	// �����Ŏg����O
	class err_ID { };					// �������̍ہA��{�`�����N�ƕ⏕�`�����N�� ID ���H������Ă���
	class err_NoCharCode { };			// �⏕�`�����N�ɕ����R�[�h��񂪂Ȃ�

public:
	const string& GetPrimaryInfo() { return m_primary; };				// ��{�������Ԃ�
	const string& GetSecondaryInfo() { return m_secondary; };			// �⏕�������Ԃ�
	const int GetPrimaryCharCode() { return m_primaryCharCode; };		// ��{������̕����R�[�h��Ԃ�
	const int GetSecondaryCharCode() { return m_secondaryCharCode; };	// �⏕������̕����R�[�h��Ԃ�

	CUniStringInfo( string id, string primary, string secondary="", int primCode=unknown_code, int scndCode=unknown_code ); // �R���X�g���N�^�A�K�v�ȏ���S�ė^����
	CUniStringInfo( string id, CHeaderManager& theManager );	// �R���X�g���N�^�A�w�b�_�}�l�[�W������ǂݏo��
	virtual ~CUniStringInfo();

	// ��O
	class err_FailConstruction { };		// �R���X�g���N�g�̎��s
};

template<class T>
T* GetPrimaryChunkFromHeaderManager( std::string id, CHeaderManager& theManager )
{

	try {
		CChunk src = theManager.GetPrimaryChunk(id);
		T* chunk = new T(src);
		return chunk;
	}
	catch ( CHeaderManager::err_FailGetChunk ) {
		return NULL;
	}
}

template<class T>
T* GetSecondaryChunkFromHeaderManager( std::string id, CHeaderManager& theManager )
{

	try {
		CChunk src = theManager.GetSecondaryChunk(id);
		T* chunk = new T(src);
		return chunk;
	}
	catch ( CHeaderManager::err_FailGetChunk ) {
		return NULL;
	}
}


#endif // !defined(AFX_HEADERMANAGER_H__E059C7C4_6C3B_11D4_A71A_00C04F012175__INCLUDED_)
