This is a radically trimmed down version of [dune-common](https://gitlab.dune-project.org/core/dune-common)
that only contains the following functionality needed for [parafields-core](https://github.com/parafields/parafields-core):

* `Dune::ParameterTree`
* `Dune::ParameterTreeParser`
* `Dune::FieldVector<T, dim>`

The project exists to ease compilation of `parafields-core` in scenarios, where `dune-common`
causes any issues. It is not meant for general use.
