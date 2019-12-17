/*
 * VMallocationBalanceFit.cc
 *
 *  Created on: 2019��7��26��
 *      Author: xiaow
 */

#include "veins/modules/edgecomputing/VM/VMallocationBalanceFit.h"
#include <math.h>
#include <numeric>
#include <vector>

bool VMallocationBalanceFit::canCreateVM(VM* vm, std::vector<PM*> PMs) {
    for (int i = 0; i < PMs.size(); i++) {
        if ((PMs[i]->getCPUR() >= vm->getMips())
                && (PMs[i]->getRamR() >= vm->getRam())) {
            return true;
        }
    }
    return false;
}

int VMallocationBalanceFit::createVMOn(VM* vm, std::vector<PM*> PMs) {
    int index = -1;
    double L = 100;
    double tempL = 0;

    for (int i = 0; i < PMs.size(); i++) {
        std::vector<double> resultSet;
        if ((PMs[i]->getCPUR() >= vm->getMips())
                && (PMs[i]->getRamR() >= vm->getRam())) {

            for (int j = 0; j < PMs.size(); j++) {
                if (j == i) {
                    tempL = PMs[j]->getPreLoadBalance(vm->getMips(),
                            vm->getRam());
                } else {
                    tempL = PMs[j]->getLoadBalance();
                }
                resultSet.push_back(tempL);
            }

        }

        double sum = 0;
        for (int k = 0; k < resultSet.size(); k++) {
            sum += resultSet[k];
        }
        double mean = sum / resultSet.size();
        sum = 0;
        for (int k = 0; k < resultSet.size(); k++) {
            sum += (resultSet[k] - mean) * (resultSet[k] - mean);
        }
        double stdev = sum / resultSet.size();
        if (stdev < L) {
            index = i;
            L = stdev;
        }
    }
    return index;
}


