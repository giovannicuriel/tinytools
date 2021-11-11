/*
 * This Kotlin source file was generated by the Gradle 'init' task.
 */
package br.giovannicuriel.tinytools.kotlin


import io.github.resilience4j.circuitbreaker.CallNotPermittedException
import io.github.resilience4j.retry.Retry
import io.github.resilience4j.retry.RetryConfig
import io.github.resilience4j.retry.RetryRegistry
import java.time.Duration
import java.util.function.Supplier

import io.github.resilience4j.circuitbreaker.CircuitBreaker
import io.github.resilience4j.circuitbreaker.CircuitBreakerRegistry
import io.github.resilience4j.kotlin.circuitbreaker.CircuitBreakerConfig


class CBExample {

    private var counter = 0;
    fun createConfig() = CircuitBreakerConfig {
        failureRateThreshold(50.0F)
        minimumNumberOfCalls(5)
        waitDurationInOpenState(Duration.ofSeconds(15))
        recordExceptions(NullPointerException::class.java)
        recordResult { response -> checkResult(response as String) }
        build()
    }

    val cb: CircuitBreaker

    init {
        val config = createConfig()
        val registry = CircuitBreakerRegistry.of(config)
        cb = registry.circuitBreaker("my-awesome-cb")
    }

    fun get(): String {
        val decorateFunction: Supplier<String> = CircuitBreaker.decorateSupplier(cb) {
            getData()
        }
        return decorateFunction.get()
    }

    private fun getData(): String {
        counter++
        val result = when (counter) {
            1 -> throw java.lang.NullPointerException("really!!!")
            2 -> "winner"
            3 -> "winner"
            4 -> "error response 4"
            else -> { // Note the block
                "winner"
            }
        }
        if (counter > 5) counter = 0
        println("Returning ${result}")
        return result

    }
    private fun checkResult(response: String): Boolean {
        return response.startsWith("error response")
    }
}


class RetryExample {
    private var counter = 0

    fun createRetryConfig() = RetryConfig.custom<Any>()
        .maxAttempts(3)
        .waitDuration(Duration.ofMillis(5000))
        .retryExceptions(NullPointerException::class.java)
        .retryOnResult { response -> checkResult(response as String) }
        .build()

    fun get(): String {
        val config = createRetryConfig()
        val registry = RetryRegistry.of(config)
        val retry: Retry = registry.retry("get-data-retry")
        val decorateFunction: Supplier<String> = Retry.decorateSupplier(retry) {
            getData()
        }

        return decorateFunction.get()
    }

    private fun getData(): String {
        counter++
        println("Trying to get data ${counter}")
        return when (counter) {
            1 -> throw java.lang.NullPointerException("really!!!")
            2 -> "error response"
            else -> { // Note the block
                return "winner"
            }
        }

    }
    private fun checkResult(response: String): Boolean {
        return response == "error response"
    }
}

class App {
    val retry = RetryExample()
}

fun main() {
//    val app = App()
//    println(app.retry.get())
    val app = CBExample()
    var counter = 0;
    while (counter < 100) {
        println("================ $counter ================")
        try {
            println(app.get())
        } catch (ex: java.lang.NullPointerException) {
            println("oh no")
        } catch (ex: CallNotPermittedException) {
            println("oh no - circuit breaker is opened!")
            Thread.sleep(3000)
        }
        counter++;
    }

}
