// fsm_commands.h
//
////////////////////////////////////////////////////////////////////////////////

#ifndef NUCLEON_FSM_COMMANDS_TEST_H_
#define NUCLEON_FSM_COMMANDS_TEST_H_

#include <quark/config.h>

//------------------------------------------------------------------------------

enum cmd_family_t
{
	map_cmd
};

enum map_command_t
{
	map_send_cmd,
	map_receive_ack_cmd
};

//------------------------------------------------------------------------------

class Command : public quark::pooled_object
{
public:
	virtual ~Command() {}
	virtual const char* name() const = 0;

	size_t family() const { return _familiy; }
	size_t id() const { return _id; }

	size_t classFamily() const { return UINT_MAX; }
	size_t classId() const { return UINT_MAX; }

protected:
	Command(size_t familiy, size_t id)
		:	_familiy(familiy), _id(id) {}

private:
	size_t _familiy;
	size_t _id;
};

template<const size_t _Family>
class CommandT : public Command
{
public:
	static size_t classFamily() { return _Family; }

protected:
	CommandT(size_t id) : Command(_Family, id) {}
};

//------------------------------------------------------------------------------

typedef CommandT<map_cmd> MapCommand;

template<const size_t _Id>
class MapCommandT : public MapCommand
{
public:
	static size_t classId() { return _Id; }

protected:
	MapCommandT() : MapCommand(_Id) {}
};

class MapSendCmd : public MapCommandT<map_send_cmd>
{
public:
	MapSendCmd() : a(55) {}
	virtual const char* name() const;
	int a;
};

class MapReceiveAckCmd : public MapCommandT<map_receive_ack_cmd>
{
public:
	virtual const char* name() const;
};

//------------------------------------------------------------------------------

#endif // NUCLEON_FSM_COMMANDS_TEST_H_
