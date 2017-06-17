#ifndef NEURAON_INTERFACE_H
#define NEURAON_INTERFACE_H

#include <iostream>
#include <map>
#include <list>
#include <vector>
#include <ctime>
#include <unordered_map>

#include "windows.h"

namespace xjoof{

	namespace tools
	{
		inline int rand(){
			static int last_value = 17961;
			LARGE_INTEGER counter;
			QueryPerformanceCounter(&counter);

			last_value = last_value ^ counter.LowPart + counter.HighPart + counter.LowPart << 16;
			return last_value;
		}
	};

	namespace neual_ai{

		typedef int ExecRtnLv;

		typedef int NextNeuronGroup;

		typedef enum{
			eNT_
		
		
		} NeuronType;


		class NeuronBase;

		class NeuronPool
		{
		public:
			static NeuronPool *Getsingleton(){
				static NeuronPool *obj_p = nullptr;
				if (!obj_p){
					obj_p = new NeuronPool();
				}
				return obj_p;
			};

			NeuronBase * GetRandNeuron(){
				return neuronPool[tools::rand() % neuronPool.size()];
			}

			void AddNeuronToPool(NeuronBase *neu_p){
				neuronPool.push_back(neu_p);
			}

			const double PoolUseRate(){
				return (double)neuronPool.size() / (double)poolMaxSize;
			}


		private:
			NeuronPool(){};
			~NeuronPool(){};

			std::vector<NeuronBase*> neuronPool;
			static const int poolMaxSize = 0xfffff;
		};



		class NextNeuronData{
		public:
			NextNeuronData(){};
			~NextNeuronData(){};

			int GetWeight(NextNeuronGroup group, NeuronBase *neuron_p, double &weight);
			void SetWeight(NextNeuronGroup group, NeuronBase *neuron_p, double &weight);

			static bool WhetherItHappened(double probability){
				const int rand_max = 0xffff;
				if (((double)rand_max * probability) >= (double)(tools::rand() & rand_max)){
					return true;
				}
				return false;
			};

			NeuronBase *GetRandomNeuronInGroup(NextNeuronGroup group){
				auto group_it = mData.find(group);

				if (group_it != mData.end()){
					double weight_sum = 0;
					for (auto neuron_it = group_it->second.begin(); neuron_it != group_it->second.end(); ++neuron_it){
						weight_sum += neuron_it->second;
					}
					weight_sum *= 1.0001;
					for (auto neuron_it = group_it->second.begin(); neuron_it != group_it->second.end(); ++neuron_it){
						if (WhetherItHappened(neuron_it->second / weight_sum)){
							return neuron_it->first;
						}
					}

					if (WhetherItHappened(NeuronPool::Getsingleton()->PoolUseRate())){
						return NeuronPool::Getsingleton()->GetRandNeuron();
					}
				}
				return nullptr;
			};


			void AdjustNeuronLink(NeuronBase *neuron_p, NextNeuronGroup group, ExecRtnLv lv){

				auto link_it = mData.find(group);

				if (link_it != mData.end()){
					auto neuron_it = link_it->second.find(neuron_p);

					if (neuron_it != link_it->second.end()){
						neuron_it->second += lv;
					}else{
						link_it->second.insert(std::make_pair(neuron_p, lv));
					}
				}else{
					std::map<NeuronBase *, double> tmp_map;
					tmp_map.insert(std::make_pair(neuron_p, lv));
					mData.insert(std::make_pair(group, tmp_map));
				}

			}

		private:
			std::map<NextNeuronGroup, std::map<NeuronBase *, double> > mData;
			 
		};


		class NeuronBase
		{
		public:

			static NeuronBase *CreateNewNeuron(const std::list <NeuronType> &mask);

			NeuronBase();
			~NeuronBase();

			virtual NeuronType GetType() = 0;

			ExecRtnLv Stimulus(const std::vector<char> &input_data){
				std::vector<char> output_data;
				NextNeuronGroup group = DoStimulus(input_data, output_data);
				NeuronBase *next_p = GetNextNeuron(group);

				if (next_p){
					ExecRtnLv rtn = next_p->Stimulus(output_data);
					allNextNeronData.AdjustNeuronLink(next_p, group, rtn);
					return rtn;
				}
				return 0;
			};

			virtual NextNeuronGroup DoStimulus(const std::vector<char> &input_data, std::vector<char> &output_data) = 0;


		private:

			
			NeuronBase *GetNextNeuron(NextNeuronGroup group){
				NeuronBase * nex_p = allNextNeronData.GetRandomNeuronInGroup(group);
				if (nex_p){
					return nex_p;
				}
				auto mask_it = linkMaskData.find(group);
				if (mask_it != linkMaskData.end()){
					return CreateNewNeuron(mask_it->second);
				}
				return CreateNewNeuron(std::list<NeuronType>());
			};

			NextNeuronData allNextNeronData;

			std::map<NextNeuronGroup, std::list<NeuronType> > linkMaskData;


		};







	};


};





#endif /*NEURAON_H*/