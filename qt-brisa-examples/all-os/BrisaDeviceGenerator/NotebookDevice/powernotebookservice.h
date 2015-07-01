#ifndef NOTEBOOKSERVICE_H_
#define NOTEBOOKSERVICE_H_

#include <BrisaUpnp/BrisaService>

using namespace Brisa;

class PowerNotebookService : public BrisaService {
public:
    PowerNotebookService();
    virtual ~PowerNotebookService();
};

#endif /* NOTEBOOKSERVICE_H_ */
