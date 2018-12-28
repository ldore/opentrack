#include "runtime-libraries.hpp"
#include "options/scoped.hpp"
#include <QMessageBox>
#include <QDebug>

runtime_libraries::runtime_libraries(QFrame* frame, dylibptr t, dylibptr p, dylibptr f)
{
    module_status status =
            module_status_mixin::error(tr("Library load failure"));

    using namespace options;

    with_tracker_teardown sentinel;

#ifdef __clang__
#   pragma clang diagnostic push
#   pragma clang diagnostic ignored "-Wcomma"
#endif

    pProtocol = make_dylib_instance<IProtocol>(p);

    if (!pProtocol)
    {
        qDebug() << "protocol dylib load failure";
        goto end;
    }

    if(status = pProtocol->initialize(), !status.is_ok())
    {
        status = tr("Error occurred while loading protocol %1\n\n%2\n")
                 .arg(p->name, status.error);
        goto end;
    }

    pTracker = make_dylib_instance<ITracker>(t);
    pFilter = make_dylib_instance<IFilter>(f);

    if (!pTracker)
    {
        qDebug() << "tracker dylib load failure";
        goto end;
    }

    if (f && !pFilter)
    {
        qDebug() << "filter load failure";
        goto end;
    }

    if (pFilter)
        if(status = pFilter->initialize(), !status.is_ok())
        {
            status = tr("Error occurred while loading filter %1\n\n%2\n")
                     .arg(f->name, status.error);
            goto end;
        }

    if (status = pTracker->start_tracker(frame), !status.is_ok())
    {
        status = tr("Error occurred while loading tracker %1\n\n%2\n")
                 .arg(t->name, status.error);
        goto end;
    }

#ifdef __clang__
#   pragma clang diagnostic pop
#endif

    correct = true;
    return;

end:
    pTracker = nullptr;
    pFilter = nullptr;
    pProtocol = nullptr;

    if (!status.is_ok())
        QMessageBox::critical(nullptr, tr("Startup failure"), status.error, QMessageBox::Cancel, QMessageBox::NoButton);
}

