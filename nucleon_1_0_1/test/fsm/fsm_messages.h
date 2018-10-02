// fsm_messages.h
//
////////////////////////////////////////////////////////////////////////////////

#ifndef NUCLEON_FSM_MESSAGES_TEST_H_
#define NUCLEON_FSM_COMMANDS_TEST_H_

#include <quark/config.h>

//------------------------------------------------------------------------------

enum msg_family_t
{
	dlg_msg,
	srv_msg
};

enum dlg_msg_id_t
{
	uabort_msg,
	pabort_msg,
	open_rsp_msg,
	close_msg,
	notice_msg
};

enum srv_msg_id_t
{
	srifsm_msg,
	srifsm_ack_msg,
	fsm_ack_msg,
	rsmds_ack_msg
};

//------------------------------------------------------------------------------

class Message : public quark::pooled_object
{
public:
	virtual ~Message() {}
	virtual const char* name() const = 0;

	size_t family() const { return _family; }
	size_t id() const { return _id; }

	static size_t classFamily() { return UINT_MAX; }
	static size_t classId() { return UINT_MAX; }

protected:
	Message(size_t family, size_t id)
		:	_family(family), _id(id) {}

private:
	size_t _family;
	size_t _id;
};

template<const size_t _Family>
class MessageT : public Message
{
public:
	static size_t classFamily() { return _Family; }

protected:
	MessageT(size_t id) : Message(_Family, id) {}
};

//------------------------------------------------------------------------------

typedef MessageT<dlg_msg> GsmDlgMessage;

template<const size_t _Id>
class GsmDlgMessageT : public GsmDlgMessage
{
public:
	static size_t classId() { return _Id; }

protected:
	GsmDlgMessageT() : GsmDlgMessage(_Id) {}
};

class GsmOpenRespDlgMsg : public GsmDlgMessageT<open_rsp_msg>
{
public:
	virtual const char* name() const;
};

class GsmCloseDlgMsg : public GsmDlgMessageT<close_msg>
{
public:
	virtual const char* name() const;
};

class GsmUAbortDlgMsg : public GsmDlgMessageT<uabort_msg>
{
public:
	virtual const char* name() const;
};

class GsmPAbortDlgMsg : public GsmDlgMessageT<pabort_msg>
{
public:
	virtual const char* name() const;
};

class GsmNoticeDlgMsg : public GsmDlgMessageT<notice_msg>
{
public:
	virtual const char* name() const;
};

//------------------------------------------------------------------------------

typedef MessageT<srv_msg> GsmSrvMessage;

template<const size_t _Id>
class GsmSrvMessageT : public GsmSrvMessage
{
public:
	static size_t classId() { return _Id; }

protected:
	GsmSrvMessageT() : GsmSrvMessage(_Id) {}
};

class GsmFsmAckMsg : public GsmSrvMessageT<fsm_ack_msg>
{
public:
	virtual const char* name() const;
};

class GsmSrifsmAckMsg : public GsmSrvMessageT<srifsm_msg>
{
public:
	virtual const char* name() const;
};

//*class GsmAlertScMsg : public GsmSrvMessage
//{
//};

class GsmRsmdsAckMsg : public GsmSrvMessageT<rsmds_ack_msg>
{
public:
	virtual const char* name() const;
};

//------------------------------------------------------------------------------

#endif // NUCLEON_FSM_MESSAGES_TEST_H_
