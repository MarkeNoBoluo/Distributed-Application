#ifndef _IDL_DATAREADER_H_INCLUDED_
#define _IDL_DATAREADER_H_INCLUDED_

#include "ReturnCode.h"
#include "BasicTypes.h"
#include "IDL_UserDataType.h"
#include "DDS/DomainParticipantFactory.h"
#include "DDS/Topic.h"
#include "DDS/Subscriber.h"
#include "DDS/DataReader.h"
#include "DDS/Listener/DataReaderListener.h"

class DataReaderImpl;

class UserDataTypeDataReader : public DataReader 
{
public:
	UserDataTypeDataReader(DataReaderImpl* pDataReaderImpl);

	~UserDataTypeDataReader();

	static UserDataTypeDataReader* narrow(DataReader* pDataReader);

	/* JDXCZ 
	访问DataReader中的数据样本合集以及相应的SampleInfo结构合集*/
	ReturnCode_t take(UserDataTypeSeq& data_values, SampleInfoSeq sample_infos, int max_samples, SampleStateKind sample_states, ViewStateKind view_states, InstanceStateKind instance_states);

	ReturnCode_t read(UserDataTypeSeq& data_values, SampleInfoSeq sample_infos, int max_samples, SampleStateKind sample_states, ViewStateKind view_states, InstanceStateKind instance_states);


	ReturnCode_t take_next_sample(UserDataType& receivedData, SampleInfo & sample_info);

};

#endif
