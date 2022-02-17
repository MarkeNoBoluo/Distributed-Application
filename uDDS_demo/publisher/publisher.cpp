/**********************************************************
!""# $%&'(
 - 13 - platforu
*****************�����˳���publisher.cpp********************
***********************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
/* IDL_TypeSupport.h�а�������������ͷ�ļ� */
#include "IDL_TypeSupport.h"
/* ɾ������ʵ�� */
static int publisher_shutdown(DomainParticipant *participant)
{
	ReturnCode_t retcode;
	int status = 0;
	if (participant != NULL) {
		retcode = participant->delete_contained_entities();
		if (retcode != RETCODE_OK) {
			fprintf(stderr, "delete_contained_entities error %d\n", retcode);
			status = -1;
		}
		retcode = DomainParticipantFactory::get_instance()->delete_participant(participant);
		if (retcode != RETCODE_OK) {
			fprintf(stderr, "delete_participant error %d\n", retcode);
			status = -1;
		}
	}
	return status;
}
/* �����ߺ��� */
extern "C" int publisher_main(int domainId, int sample_count)
{
	DomainParticipant *participant = NULL;
	Publisher *publisher = NULL;
	Topic *topic = NULL;
	DataWriter *writer = NULL;
	UserDataTypeDataWriter * UserDataType_writer = NULL;
	UserDataType *instance = NULL;
	ReturnCode_t retcode;
	InstanceHandle_t instance_handle = HANDLE_NIL;
	const char *type_name = NULL;
	int count = 0;
	/* 1. ����һ��participant�������ڴ˴�����participant��QoS */
	/* ����1���ڳ������������ȴ���participant��������Դ��ʼ��*/
	/* ����2����ͬ��domainIdֻ����һ��participant���ظ�������ռ�ô�����Դ */
	participant = DomainParticipantFactory::get_instance()->create_participant(
		domainId, PARTICIPANT_QOS_DEFAULT/* participantĬ��QoS */,
		NULL /* listener */, STATUS_MASK_NONE);
	if (participant == NULL) {
		fprintf(stderr, "create_participant error\n");
		publisher_shutdown(participant);
		return -1;
	}
	/* 2. ����һ��publisher�������ڴ���publisher��ͬʱ������QoS */
	/* ����1���ڳ������������ȴ���publisher */
	/* ����2��һ��participant�´���һ��publisher���ɣ������ظ����� */
	publisher = participant->create_publisher(
		PUBLISHER_QOS_DEFAULT /* Ĭ��QoS */,
		NULL /* listener */, STATUS_MASK_NONE);
	if (publisher == NULL) {
		fprintf(stderr, "create_publisher error\n");
		publisher_shutdown(participant);
		return -1;
	}
	/* 3. �ڴ�������֮ǰע���������� */
	/* ����1���ڳ������������Ƚ���ע�� */
	/* ����2��һ����������ע��һ�μ��� */
	type_name = UserDataTypeTypeSupport::get_type_name();
	retcode = UserDataTypeTypeSupport::register_type(
		participant, type_name);
	if (retcode != RETCODE_OK) {
		fprintf(stderr, "register_type error %d\n", retcode);
		publisher_shutdown(participant);
		return -1;
	}
	/* 4. �������⣬�����������QoS */
	/* ����1���ڳ������������ȴ���Topic */
	/* ����2��һ������������һ�μ��ɣ������ظ����� */
	topic = participant->create_topic(
		"Example UserDataType"/* ������ */,
		type_name /* ������ */, TOPIC_QOS_DEFAULT/* Ĭ��QoS */,
		NULL /* listener */, STATUS_MASK_NONE);
	if (topic == NULL) {
		fprintf(stderr, "create_topic error\n");
		publisher_shutdown(participant);
		return -1;
	}
	/* 5. ����datawriter��������datawriter��QoS */
	/* ����1���ڳ������������ȴ���datawriter */
	/* ����2������һ�μ��ɣ������ظ����� */
	/* ����3���ڳ����˳�ʱ�ٽ����ͷ� */
	/* ����4��������㷢������ʱ����datawriter���������ݺ�ɾ����������������Դ��Ӱ������ */
	writer = publisher->create_datawriter(
		topic, DATAWRITER_QOS_DEFAULT/* Ĭ��QoS */,
		NULL /* listener */, STATUS_MASK_NONE);
	if (writer == NULL) {
		fprintf(stderr, "create_datawriter error\n");
		publisher_shutdown(participant);
		return -1;
	}
	UserDataType_writer = UserDataTypeDataWriter::narrow(writer);
	if (UserDataType_writer == NULL) {
		fprintf(stderr, "DataWriter narrow error\n");
		publisher_shutdown(participant);
		return -1;
	}
	/* 6. ����һ���������� */
	/* ���飺������Ϊnew�����ģ�ʹ�ú��û���Ҫ�Լ�delete */
	instance = UserDataTypeTypeSupport::create_data();
	if (instance == NULL) {
		fprintf(stderr, "UserDataTypeTypeSupport::create_data error\n");
		publisher_shutdown(participant);
		return -1;
	}
	/* 7. ��ѭ�� ���������� */
	char *cColor = new char[100];
	for (count = 0; (sample_count == 0) || (count < sample_count); ++count) {
		/* �ڴ˴��޸����� */
		//scanf_s("%hd %hd %s", &instance->x, &instance->y, cColor,100);
		instance->color = cColor;
		instance->x = 3 + count;
		instance->y = 4 + count;
		strcpy_s(cColor, 5, "red");
		retcode = UserDataType_writer->write(*instance, instance_handle);
		if (retcode != RETCODE_OK) {
			fprintf(stderr, "write error %d\n", retcode);
		}
		Sleep(1000);//��˯1��
	}
	delete cColor;
	/* 8. ɾ���������� */
	retcode = UserDataTypeTypeSupport::delete_data(instance);
	if (retcode != RETCODE_OK) {
		fprintf(stderr, "UserDataTypeTypeSupport::delete_data error %d\n", retcode);
	}
	/* 9. ɾ������ʵ�� */
	return publisher_shutdown(participant);
}
int main(int argc, char *argv[])
{
	int domain_id = 0;
	int sample_count = 0; /* ����ѭ�� */
	if (argc >= 2) {
		domain_id = atoi(argv[1]); /* ��������domain_id */
	}
	if (argc >= 3) {
		sample_count = atoi(argv[2]); /* ����sample_count�� */
	}
	return publisher_main(domain_id, sample_count);
}