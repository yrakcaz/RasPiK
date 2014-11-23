.global vector

.extern treat_reset
.extern treat_undef
.extern treat_swi
.extern treat_pref_abort
.extern treat_data_abort
.extern treat_unused
.extern treat_irq
.extern treat_fiq

vector:
    b treat_reset
    b treat_undef
    b treat_swi
    b treat_pref_abort
    b treat_data_abort
    b treat_unused
    b treat_irq
    b treat_fiq
