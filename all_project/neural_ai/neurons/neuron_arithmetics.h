#ifndef NEURAON_ARITHMETICS_H
#define NEURAON_ARITHMETICS_H

#include "neuron_interface.h"

namespace xjoof{

	namespace neual_ai{




		template <class OperandT>
		class NeuronAdd : public NeuronBase
		{
		public:
			NeuronAdd();
			~NeuronAdd();

			ExecRtnLv Stimulus(const char *input_data, int input_len, char *output_data, int *output_len){
				const int operand_num = input_len / sizeof(OperandT);
				OperandT *operand_ptr = (OperandT *)input_data;
				if (!input_len || (len - (operand_num * sizeof(OperandT)))){
					return 0;
				}
				for (int operand_i = 0; operand_i < operand_num; ++operand_i)
					*(OperandT*)output_data += operand_ptr[operand_i];
			}
			*output_len = sizeof(OperandT);
		};



		template <class OperandT>
		class NeuronMulti : public NeuronBase
		{
		public:
			NeuronAdd();
			~NeuronAdd();

			ExecRtnLv Stimulus(const char *input_data, int input_len, char *output_data, int *output_len){
				const int operand_num = input_len / sizeof(OperandT);
				OperandT *operand_ptr = (OperandT *)input_data;
				if (!input_len || (len - (operand_num * sizeof(OperandT)))){
					return 0;
				}
				for (int operand_i = 0; operand_i < operand_num; ++operand_i)
					*(OperandT*)output_data *= operand_ptr[operand_i];
			}
			*output_len = sizeof(OperandT);
		};

	};



};





#endif /*NEURAON_H*/